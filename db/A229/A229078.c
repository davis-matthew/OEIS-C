/*
 * A229078  Number of ascending runs in {1,...,n}^n.
 * https://oeis.org/A229078
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^(n-1)*(n*(n+2)-1)/2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t n_seq = (seq_t)n;
    seq_t exp = (seq_t)(n - 1);

    /* compute n^(n-1) */
    seq_t power = big_pow(n_seq, exp);

    /* compute (n*(n+2)-1) */
    seq_t t2 = n_seq * ((seq_t)n + (seq_t)2) - (seq_t)1;

    /* multiply and divide by 2 */
    seq_t result = power * t2;
    result /= (seq_t)2;

    return result;
}
