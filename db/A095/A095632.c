/*
 * A095632  a(n) = 121 written in base n.
 * https://oeis.org/A095632
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 121 written in base n
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 none, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Base‑1: unary representation */
    if (n < 1) return 0;
    if (n == 1) {
        seq_t res = 0;
        for (idx_t i = 0; i < 121; ++i) {
            res = res * 10 + 1;
        }
        return res;
    }

    /* Convert 121 to base n and concatenate its digits in decimal */
    idx_t tmp = 121;
    idx_t digits[16];   /* maximum possible digits for n ≥ 2 */
    int dlen = 0;

    while (tmp > 0) {
        digits[dlen++] = tmp % n;
        tmp /= n;
    }

    seq_t res = 0;
    for (int i = dlen - 1; i >= 0; --i) {
        idx_t d = digits[i];
        int len = 1;
        if (d >= 1000)       len = 4;
        else if (d >= 100)   len = 3;
        else if (d >= 10)    len = 2;

        seq_t mult = 1;
        for (int k = 0; k < len; ++k) mult *= 10;

        res = res * mult + d;
    }
    return res;
}
