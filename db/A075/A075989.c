/*
 * A075989  Number of k satisfying 1<=k<=n and {n/k} >= 1/2, where {n/k} is
 *          the fractional part of n/k, i.e., {n/k} = n/k - floor(n/k).
 * https://oeis.org/A075989
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} (floor(2*n/(2*k+1)) - floor(2*n/(2*k+2)))
 * Range:     u8 n <= 78, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t count = 0;
    seq_t two_n = (seq_t)n * (seq_t)2;          /* 2*n */
    for (idx_t k = 1; k <= n; ++k) {
        seq_t denom1 = (seq_t)(2 * k) + 1;      /* 2*k + 1 */
        seq_t denom2 = denom1 + 1;              /* 2*k + 2 */
        seq_t term1 = two_n / denom1;
        seq_t term2 = two_n / denom2;
        count += term1 - term2;
    }
    return count;
}
