/*
 * A075831  Like A073327, but multiply the numerical values of the letters
 *          instead of adding them.
 * https://oeis.org/A075831
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, word
 * Formula:   a(n) = product of letter values in the English words that
 *            represent the integer n (a=1, b=2, …, z=26). The words are
 *            written as in standard spell‑out notation ("one",
 *            "twenty‑three", etc.) and the numeric products of the individual
 *            words are multiplied together
 * Range:     u8 none, u16 n <= 2, u32 n <= 20, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   product_of_string
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/product_of_string.h"

seq_t a(idx_t n)
{
    static bool inited = false;
    static seq_t unit_prod[20];
    static seq_t tens_prod[10];
    static seq_t scale_prod[7];
    static seq_t prod_hundred;

    if (!inited) {
        inited = true;

        /* units 0 … 19 */
        const char *units_str[20] = {
            "zero","one","two","three","four","five","six","seven","eight",
            "nine","ten","eleven","twelve","thirteen","fourteen","fifteen",
            "sixteen","seventeen","eighteen","nineteen"
        };
        for (int i = 0; i < 20; ++i)
            unit_prod[i] = product_of_string(units_str[i]);

        /* tens 20 … 90 */
        const char *tens_str[10] = { "", "", "twenty","thirty","forty",
                                     "fifty","sixty","seventy","eighty","ninety" };
        for (int i = 0; i < 10; ++i)
            tens_prod[i] = product_of_string(tens_str[i]);

        /* scale words: thousand, million, billion, trillion,
           quadrillion, quintillion */
        const char *scale_str[7] = { "", "thousand", "million", "billion",
                                     "trillion", "quadrillion", "quintillion" };
        for (int i = 0; i < 7; ++i)
            scale_prod[i] = product_of_string(scale_str[i]);
        scale_prod[0] = 1;   /* identity for the units group */

        /* product of the word "hundred" */
        prod_hundred = product_of_string("hundred");
    }

    if (n == 0)
        return unit_prod[0];      /* "zero" */

    /* break number into groups of up to 999 (thousands, millions, …) */
    idx_t groups[7];
    size_t gcnt = 0;
    idx_t tmp = n;
    while (tmp > 0) {
        groups[gcnt++] = tmp % 1000;
        tmp /= 1000;
    }

    seq_t result = 1;

    /* process groups from the highest scale downwards */
    for (int i = (int)gcnt - 1; i >= 0; --i) {
        idx_t g = groups[i];
        if (g == 0)
            continue;            /* no words for this scale */

        /* product for the 0–999 part of the group */
        seq_t gprod = 1;

        if (g >= 100) {
            idx_t hundreds = g / 100;
            gprod *= unit_prod[hundreds];   /* unit word for the hundreds */
            gprod *= prod_hundred;          /* "hundred" */
            g -= hundreds * 100;
        }

        if (g >= 20) {
            idx_t tens = g / 10;
            gprod *= tens_prod[tens];
            g %= 10;
        }

        if (g > 0) {                     /* units or teens */
            gprod *= unit_prod[g];
        }

        result *= gprod;
        if (i > 0)                       /* multiply by scale word if any */
            result *= scale_prod[i];
    }

    return result;
}
