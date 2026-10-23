/*
 * A178922  a(n) = (n+1)^n - n^(n-1) for n > 0, a(0) = 1.
 * https://oeis.org/A178922
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1)^n - n^(n-1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0)
        return (seq_t)1;

    seq_t n_seq  = (seq_t)n;
    seq_t n1_seq = n_seq + (seq_t)1;

    /* (n+1)^n */
    seq_t term1 = big_pow(n1_seq, n_seq);

    /* n^(n-1)  */
    seq_t term2 = big_pow(n_seq, n_seq - (seq_t)1);

    return term1 - term2;
}
