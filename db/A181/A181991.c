/*
 * A181991  n-alternating permutations of length 4n.
 * https://oeis.org/A181991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = -1 + 2*binom(4*n, n) + binom(4*n, 2*n) - 3*binom(4*n,
 *            2*n)*binom(2*n, n) + binom(4*n, n)*binom(3*n, n)*binom(2*n, n)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 14,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    idx_t four_n  = 4 * n;
    idx_t two_n   = 2 * n;
    idx_t three_n = 3 * n;

    seq_t B4n_n  = binom(four_n, n);    /* binom(4n, n) */
    seq_t B4n_2n = binom(four_n, two_n);/* binom(4n, 2n) */
    seq_t B3n_n  = binom(three_n, n);  /* binom(3n, n) */
    seq_t B2n_n  = binom(two_n, n);    /* binom(2n, n) */

    /* Positive part: 2*B4n_n + B4n_2n + B4n_n*B3n_n*B2n_n */
    seq_t pos = (seq_t)2 * B4n_n + B4n_2n + B4n_n * B3n_n * B2n_n;

    /* Negative part: 3*B4n_2n*B2n_n + 1 */
    seq_t neg = (seq_t)3 * B4n_2n * B2n_n + (seq_t)1;

    /* For n >= 1 the positive part is always >= the negative part. */
    return (pos < neg) ? (seq_t)0 : (pos - neg);
}
