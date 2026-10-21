/*
 * A095492  a(n) = 50 written in base n.
 * https://oeis.org/A095492
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = decimal concatenation of the base‑n representation of 50
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 none, big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow10.h"

/* seq_t and idx_t are defined by the build system */
seq_t a(idx_t n)
{
    /* Special case: base‑1 (unary) */
    if (n == 1) {
        seq_t res = 0;
        for (idx_t i = 0; i < 50; ++i)
            res = res * 10 + 1;
        return res;
    }

    /* Compute the digits of 50 in base n (least significant first) */
    idx_t temp = 50;
    int digits[64];          /* max digits needed are few */
    int len = 0;
    while (temp > 0) {
        digits[len++] = temp % n;
        temp /= n;
    }

    /* Build the decimal concatenation of those digits */
    seq_t res = 0;
    for (int i = len - 1; i >= 0; --i) {
        unsigned d = digits[i];
        /* Count decimal digits of d (d < n <= 10000) */
        int dlen = 1;
        unsigned dd = d;
        while (dd >= 10) {
            ++dlen;
            dd /= 10;
        }
        res = res * pow10((idx_t)dlen) + d;
    }
    return res;
}
