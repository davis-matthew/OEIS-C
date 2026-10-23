/*
 * A174060  a(n) = Sum_{k=0..n} floor(sqrt(k))^2.
 * https://oeis.org/A174060
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} floor(sqrt(k))^2 = (1/6) * m * ( 6*m*(n+1) -
 *            (m+1)*( 3*m*m + m - 1 ) ) where m = floor(sqrt(n))
 * Range:     u8 n <= 20, u16 n <= 373, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t floor_sqrt_u64(idx_t n)
{
    idx_t low = 0;
    idx_t high = ((idx_t)1ULL << 32);   /* high > sqrt(max uint64_t) */
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if (mid * mid <= n) {
            low = mid;
        } else {
            high = mid - 1;
        }
    }
    return low;
}

seq_t a(idx_t n)
{
    idx_t m = floor_sqrt_u64(n);
    if (m == 0) return (seq_t)0;

    seq_t M = (seq_t)m;
    seq_t N = (seq_t)n;

    /* 6*m*(n+1) */
    seq_t term1 = 6 * M * (N + 1);

    /* (m+1)*(3*m*m + m - 1) */
    seq_t term2 = (M + 1) * (3 * M * M + M - 1);

    seq_t diff = term1 - term2;            /* divisible by 6 */
    seq_t res  = M * diff / 6;             /* final result */

    return res;
}
