/*
 * A028391  a(n) = n - floor(sqrt(n)).
 * https://oeis.org/A028391
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = n - floor(sqrt(n))
 * Range:     u8 n <= 271, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square‑root of an unsigned 64‑bit value.
 * Returns floor(sqrt(n)). */
static inline idx_t isqrt_idx(idx_t n)
{
    idx_t lo = 0;
    idx_t hi = 0xFFFFFFFFULL;            /* 32‑bit max, sqrt fits in 32 bits */

    while (lo < hi) {
        idx_t mid = lo + ((hi - lo + 1) >> 1);   /* upper mid to avoid infinite loop */
        if (mid * mid <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    idx_t s = isqrt_idx(n);           /* floor(sqrt(n)) */
    return (seq_t)n - (seq_t)s;       /* n - floor(sqrt(n)) */
}
