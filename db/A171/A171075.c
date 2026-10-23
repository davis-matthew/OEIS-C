/*
 * A171075  Total number of balls on lawn in variant of the tennis ball
 *          problem (cf. A171074).
 * https://oeis.org/A171075
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1; a(n)=n*(2n+1)*C(2n-1,n-1)-2n^2-2 for n>=2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   binom_n_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom_n_k.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    seq_t sn = (seq_t)n;
    /* n * (2n+1) */
    seq_t mul = sn * ((((seq_t)2) * sn) + (seq_t)1);

    /* binomial coefficient C(2n-1, n-1) */
    idx_t two_n_minus1 = (idx_t)(2 * n - 1);
    idx_t n_minus1     = (idx_t)(n - 1);
    seq_t bin = binom_n_k(two_n_minus1, n_minus1);

    seq_t total = mul * bin;

    /* 2n^2 + 2 */
    seq_t sub = (((seq_t)2) * sn * sn) + (seq_t)2;

    return total - sub;
}
