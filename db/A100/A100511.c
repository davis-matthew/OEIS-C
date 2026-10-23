/*
 * A100511  a(n) = Sum_{j=0..n} Sum_{k=0..n}
 *          binomial(n,j)*binomial(n,k)*max(j,k).
 * https://oeis.org/A100511
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*2^(2*n-1) + (n/2)*binomial(2*n, n)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 29, u128 n <= 61,
 *            big any n
 * Helpers:   binom_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom_uint64.h"

seq_t a(idx_t n)
{
    /* a(0) = 0 */
    if (n == 0) {
        return (seq_t)0;
    }

    /* compute binomial(2*n, n) */
    idx_t two_n = n << 1;                     /* 2*n */
    seq_t binom = binom_uint64((uint64_t)two_n, (uint64_t)n);

    /* (n * binom) / 2  --- binom is always even for n > 0 */
    seq_t part2 = (binom / (seq_t)2) * (seq_t)n;

    /* compute 2^(2*n-1) by repeated multiplication by 2 */
    seq_t pow2 = (seq_t)1;
    for (idx_t i = 0; i < two_n - 1; ++i) {
        pow2 *= (seq_t)2;
    }

    /* n * 2^(2*n-1) */
    seq_t part1 = (seq_t)n * pow2;

    /* final result */
    return part1 + part2;
}
