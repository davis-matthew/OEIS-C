/*
 * A050686  Number of palindromes of length n and containing the digit 1 (or
 *          any other fixed nonzero digit).
 * https://oeis.org/A050686
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 9*10^⌊(n-1)/2⌋ - 8*9^⌊(n-1)/2⌋ (for n ≥ 1)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   pow_seq_const
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq_const.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not defined for n=0 in OEIS, return 0 */
    idx_t k = (n - 1) / 2;         /* floor((n-1)/2) */
    seq_t ten  = 10;
    seq_t nine = 9;
    seq_t p10 = pow_seq_const(ten,  k);
    seq_t p9  = pow_seq_const(nine, k);
    seq_t res1 = 9 * p10;
    seq_t res2 = 8 * p9;
    return res1 - res2;
}
