/*
 * A042533  Denominators of continued fraction convergents to sqrt(795).
 * https://oeis.org/A042533
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 13252*a(n-4) - a(n-8), for n > 7 initial values: a(0)=1,
 *            a(1)=5, a(2)=46, a(3)=235, a(4)=13206, a(5)=66265, a(6)=609591,
 *            a(7)=3114220
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 37,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    static const seq_t init[8] = {
        (seq_t)1,      /* a(0) */
        (seq_t)5,      /* a(1) */
        (seq_t)46,     /* a(2) */
        (seq_t)235,    /* a(3) */
        (seq_t)13206,  /* a(4) */
        (seq_t)66265,  /* a(5) */
        (seq_t)609591, /* a(6) */
        (seq_t)3114220 /* a(7) */
    };

    if (n < 8)
        return init[n];

    seq_t buffer[8];
    size_t i;
    for (i = 0; i < 8; ++i)
        buffer[i] = init[i];

    for (idx_t i2 = 8; i2 <= n; ++i2) {
        idx_t idx_cur   = i2 & 7;          /* i2 % 8 */
        idx_t idx_prev4 = (i2 - 4) & 7;    /* (i2-4) % 8 */
        seq_t a_prev4   = buffer[idx_prev4];
        seq_t a_prev8   = buffer[idx_cur]; /* holds a(i2-8) */
        seq_t val       = mul_small(a_prev4, 13252) - a_prev8;
        buffer[idx_cur] = val;
    }

    return buffer[n & 7];
}
