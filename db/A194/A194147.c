/*
 * A194147  a(n) = Sum_{j=1..n} floor(j*(-1+sqrt(6))); n-th partial sum of
 *          Beatty sequence for -1+sqrt(6).
 * https://oeis.org/A194147
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{j=1..n} floor(j*(-1+sqrt(6))) * = Sum_{j=1..n} (
 *            floor(j*sqrt(6)) - j ) * = Sum_{j=1..n} floor(j*sqrt(6)) -
 *            n*(n+1)/2
 * Range:     u8 n <= 18, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer square root for unsigned 64-bit numbers */
static inline uint64_t isqrt_u64(uint64_t n)
{
    uint64_t r = 0;
    uint64_t bit = 1ULL << 62;              /* largest power of 4 <= 2^64-1 */
    while (bit > n) bit >>= 2;
    while (bit) {
        if (n >= r + bit) {
            n -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    /* sum of floor(j*sqrt(6)) for j = 1..n */
    seq_t sum_sqrt = 0;
    for (idx_t j = 1; j <= n; ++j) {
        uint64_t jj = (uint64_t)j;
        uint64_t x  = 6ULL * jj * jj;   /* compute 6*j*j */
        uint64_t t  = isqrt_u64(x);     /* floor(j*sqrt(6)) */
        sum_sqrt += (seq_t)t;
    }
    /* subtract the triangular number n*(n+1)/2 */
    seq_t triangular = (seq_t)n * (seq_t)(n + 1) / 2;
    return sum_sqrt - triangular;
}
