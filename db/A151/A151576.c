/*
 * A151576  Number of permutations of 1..n arranged in a circle with exactly 3
 *          adjacent element pairs in decreasing order.
 * https://oeis.org/A151576
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * ( 3^(n-1) - n * 2^(n-1) + n*(n-1)/2 )
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 38, u128 n <= 77,
 *            big any n
 * Helpers:   pow_uint_seqs
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint_seqs.h"

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;        /* sequence is defined only for n ≥ 3 */

    idx_t nm1 = n - 1;
    seq_t n_seq = (seq_t)n;

    seq_t pow2 = pow_uint_seqs(2, nm1);
    seq_t pow3 = pow_uint_seqs(3, nm1);

    seq_t term2 = n_seq * pow2;                     /* n * 2^(n-1) */
    seq_t term3 = n_seq * (n_seq - 1) / 2;          /* n*(n-1)/2     */

    seq_t inner = pow3 - term2 + term3;
    return n_seq * inner;
}
