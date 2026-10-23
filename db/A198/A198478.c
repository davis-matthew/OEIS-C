/*
 * A198478  a(n) = 9^n * n^9.
 * https://oeis.org/A198478
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9^n * n^9
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 10, u128 n <= 26,
 *            big any n
 * Helpers:   big_pow, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    /* Compute 9^n */
    seq_t pow9 = big_pow((seq_t)9, (seq_t)n);

    /* Compute n^9 */
    seq_t pown9 = pow_seq((seq_t)n, 9);

    /* Return the product */
    return pow9 * pown9;
}
