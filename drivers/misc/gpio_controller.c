 /****************************************************************************************
 * File:			drivers/misc/gpio_controller.c
 * Copyright:		Copyright (C) 2014-2015 RK Corporation.
 * Author:			qin <qin@armdesigner.com>
 * Date:			2015.11.13
 * Description:		This driver use for rk32 chip usb sata. Use i2c IF ,the chip is 
 * 				    JM20329E_LGCA
 *****************************************************************************************/

#include <dt-bindings/gpio/gpio.h>
#include <linux/input.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/kernel.h>
#include <linux/fcntl.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/miscdevice.h>
#include <asm/types.h>
#include <linux/platform_device.h>
#include <asm/uaccess.h>
#include <linux/wait.h>
#include <linux/of_gpio.h>
#include <linux/gpio.h>
#include <linux/slab.h>

#define DEVICE_NAME "gpios"

static struct of_device_id gpio_controller_of_match[] = {
	{ .compatible = "gpio_controller" },
	{ }
};

MODULE_DEVICE_TABLE(of, gpio_controller_of_match);

static int gpio_controller_probe(struct platform_device *pdev)
{
	struct device_node *node = pdev->dev.of_node;
	enum of_gpio_flags flags;
	int  ret,mini4g_en,value,sata_en,fan_en;
//	printk("func: %s\n", __func__); 
	if (!node)
		return -ENODEV;
	
	mini4g_en= of_get_named_gpio_flags(node, "mini4g_en", 0, &flags);
	
	ret = gpio_request(mini4g_en, "mini4g-en");
	if (ret) {
		printk("gpio_controller,failed to request mini4g-en\n");
	}	
	gpio_direction_output(mini4g_en, 1);
mdelay(10);
	//gpio_direction_output(pcie_en, 1);

 	value = gpio_get_value(mini4g_en);
	
	printk("gpio_controller_probe value=%d mini4g_en=%d ok!\n",value,mini4g_en);

	sata_en= of_get_named_gpio_flags(node, "sata_en", 0, &flags);
	
	ret = gpio_request(sata_en, "sata-en");
	if (ret) {
		printk("gpio_controller,failed to request sata_en\n");
		//return -EINVAL;
	}	
	gpio_direction_output(sata_en, 1);
	
	fan_en= of_get_named_gpio_flags(node, "fan_en", 0, &flags);
	
	ret = gpio_request(fan_en, "fan-en");
	if (ret) {
		printk("gpio_controller,failed to request fan_en\n");
		//return -EINVAL;
	}	
	gpio_direction_output(fan_en, 1);

	value = gpio_get_value(sata_en);

	printk("gpio_controller_probe value=%d sata_en=%d ok!\n",value,sata_en);

	return 0;
}

static int gpio_controller_remove(struct platform_device *pdev)
{
//	printk("func: %s\n", __func__); 

	return 0;
}
static struct platform_driver gpio_controller_driver = {
	.driver		= {
		.name		= "_gpio_controller",
		.owner		= THIS_MODULE,
		.of_match_table	= of_match_ptr(gpio_controller_of_match),
	},
	.probe		= gpio_controller_probe,
	.remove		= gpio_controller_remove,
};

module_platform_driver(gpio_controller_driver);

MODULE_DESCRIPTION("gpio_controller");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("qin<qin@armdesigner.com>");
MODULE_ALIAS("platform: rk3288");
