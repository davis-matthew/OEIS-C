/*
 * A095435  a(n) = 20 written in base n.
 * https://oeis.org/A095435
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = representation of 20 in base n (decimal digits
 *            concatenated)
 * Range:     u8 none, u16 none, u32 none, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t pow10_int(int k)
{
    seq_t r = 1;
    for (int i = 0; i < k; ++i)
        r *= 10;
    return r;
}

seq_t a(idx_t n)
{
    /* Special case: base 1 (unary), 20 ones */
    if (n == 1)
    {
        seq_t r = 0;
        for (idx_t i = 0; i < 20; ++i)
            r = r * 10 + 1;
        return r;
    }

    /* Base-n digits of the number 20 */
    seq_t x = (seq_t)20;
    seq_t digit[32];
    int dcnt = 0;
    while (x > 0)
    {
        digit[dcnt++] = (seq_t)(x % n);
        x /= n;
    }

    /* Build the decimal number from the digits (most significant first) */
    seq_t res = 0;
    for (int i = dcnt - 1; i >= 0; --i)
    {
        seq_t d = digit[i];
        /* Count decimal digits of the base‑n digit */
        seq_t t = d;
        int len = 0;
        do
        {
            ++len;
            t /= 10;
        } while (t > 0);
        res = res * pow10_int(len) + d;
    }
    return res;
}
