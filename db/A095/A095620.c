/*
 * A095620  a(n) = 115 written in base n.
 * https://oeis.org/A095620
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 115 written in base n (interpreted as a decimal integer)
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 none, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Base 1 is unary: 115 ones. */
    if (n == 1) {
        seq_t val = 0;
        for (idx_t i = 0; i < 115ULL; ++i) {   /* 115 consecutive 1's */
            val = val * 10 + 1;
        }
        return val;
    }

    /* Compute the digits of 115 in base n (least significant first). */
    idx_t tmp = 115ULL;
    int digits[128];   /* sufficient for any base (max ~115 digits for base 2). */
    int dcnt = 0;
    while (tmp > 0) {
        digits[dcnt++] = (int)(tmp % n);
        tmp /= n;
    }

    /* Build the decimal value by concatenating the digit representations. */
    seq_t val = 0;
    for (int i = dcnt - 1; i >= 0; --i) {
        int d = digits[i];
        if (d < 10) {
            val = val * 10 + d;
        } else if (d < 100) {            /* two‑digit decimal */
            val = val * 10 + d / 10;
            val = val * 10 + d % 10;
        } else {                         /* d can be at most 115 -> three digits */
            val = val * 10 + d / 100;
            val = val * 10 + (d / 10) % 10;
            val = val * 10 + d % 10;
        }
    }
    return val;
}
