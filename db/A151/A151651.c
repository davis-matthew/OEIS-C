/*
 * A151651  Number of permutations of 6 indistinguishable copies of 1..n with
 *          exactly 2 adjacent element pairs in decreasing order.
 * https://oeis.org/A151651
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 28^n - (6*n + 1)*7^n + 3*n*(6*n + 1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* cast the index to seq_t for arithmetic */
    seq_t n_seq = (seq_t)n;

    /* compute 28^n and 7^n using the utility function */
    seq_t pow28 = big_pow((seq_t)28, n_seq);
    seq_t pow7  = big_pow((seq_t)7, n_seq);

    /* intermediate values */
    seq_t six_n  = (seq_t)6 * n_seq;          /* 6*n */
    seq_t factor = six_n + (seq_t)1;         /* 6*n + 1 */

    seq_t term2 = factor * pow7;             /* (6*n + 1)*7^n */
    seq_t term3 = (seq_t)3 * n_seq * factor; /* 3*n*(6*n + 1) */

    /* result = 28^n + 3*n*(6*n + 1) - (6*n + 1)*7^n */
    seq_t result = pow28 + term3 - term2;

    return result;
}
