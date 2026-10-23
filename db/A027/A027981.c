/*
 * A027981  a(n) = Sum_{k=0..2*n} (k+1)*T(n,k), T given by A027960.
 * https://oeis.org/A027981
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3 * 2^n * (2*n-1) + 4 (a(0)=1)
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 24, u64 n <= 55, u128 n <= 118,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)1;
    }

    /* compute 2^n */
    seq_t pow = (seq_t)1;
    for (idx_t i = 0; i < n; ++i) {
        pow <<= 1;               /* pow *= 2; using shift for all seq_t types */
    }

    /* compute factor (2*n-1) using signed 64-bit intermediate to avoid
       underflow for n == 0 (handled already above) */
    seq_t factor = (seq_t)((int64_t)2 * n - 1);

    seq_t res = pow * factor;   /* 2^n * (2n-1) */
    res *= (seq_t)3;            /* multiply by 3 */
    res += (seq_t)4;            /* add 4 */

    return res;
}
