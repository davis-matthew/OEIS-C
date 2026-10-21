/*
 * A233411  The number of length n binary words with some prefix which
 *          contains two more 1's than 0's or two more 0's than 1's.
 * https://oeis.org/A233411
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n - 2^ceil(n/2)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* total number of binary words of length n */
    seq_t total = pow2(n);

    /* exponent for the ceiling half: ceil(n/2) == (n+1)/2 */
    idx_t half_exp = (n + 1u) / 2u;
    seq_t half = pow2(half_exp);

    return total - half;
}
