/*
 * A095640  a(n) = 125 written in base n.
 * https://oeis.org/A095640
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = decimal representation of 125 written in base n For n = 1
 *            the representation is 125 ones; for n > 125 it is the single
 *            digit 125. Implementation converts 125 to base n and builds the
 *            decimal number formed by concatenating the base‑n digits
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 none, big any n
 * Helpers:   int_pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/int_pow10.h"

seq_t a(idx_t n)
{
    /* Handle the special unary case (base 1). */
    if (n == (idx_t)1)
    {
        seq_t res = 0;
        for (unsigned i = 0; i < 125; ++i)
            res = res * 10 + 1;
        return res;
    }

    /* For n > 125 the number 125 is less than the base, so the
       representation is a single digit '125'. */
    if (n > (idx_t)125)
        return (seq_t)125;

    /* Convert 125 to base n (n <= 125). */
    seq_t num = (seq_t)125;
    seq_t digits[128];
    int dcnt = 0;

    while (num > 0)
    {
        seq_t d = num % (seq_t)n;
        digits[dcnt++] = d;
        num /= (seq_t)n;
    }

    if (dcnt == 0) /* shouldn't happen for n>=2 */
    {
        return (seq_t)0;
    }

    /* Build the result by concatenating the decimal strings of
       each digit (most‑significant first). */
    seq_t result = 0;
    for (int i = dcnt - 1; i >= 0; --i)
    {
        seq_t d = digits[i];
        int width;
        if (d >= 100)
            width = 3;
        else if (d >= 10)
            width = 2;
        else
            width = 1;

        result = result * int_pow10(width) + d;
    }

    return result;
}
