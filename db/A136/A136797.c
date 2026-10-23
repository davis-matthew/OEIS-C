/*
 * A136797  Number of labeled marked trees with n nodes.
 * https://oeis.org/A136797
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(2*n-1) * n^(n-2)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 12, u128 n <= 21,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Handle the special case n = 1 where n^(n-2) would be 1^(-1) */
    if (n == (idx_t)1)
        return (seq_t)2;

    /* Exponent for the power of 2: 2*n - 1 */
    idx_t exp_pow2 = (idx_t)(2 * n - (idx_t)1);
    /* Exponent for the power of n: n - 2 */
    idx_t exp_pown = (idx_t)(n - (idx_t)2);

    /* Compute powers using the provided big_pow utility */
    seq_t pow2 = big_pow((seq_t)2, (seq_t)exp_pow2);
    seq_t pown = big_pow((seq_t)n, (seq_t)exp_pown);

    /* The sequence value is the product of the two powers */
    return pow2 * pown;
}
