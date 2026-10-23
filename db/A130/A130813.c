/*
 * A130813  If X_1,...,X_n is a partition of a 2n-set X into 2-blocks then
 *          a(n) is equal to the number of 7-subsets of X containing none of
 *          X_i, (i=1,...n).
 * https://oeis.org/A130813
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 128 * binomial(n,7) (for n >= 7)
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* For indices below the offset, the sequence is not defined.
       Return 0 so the function has a defined behaviour. */
    if (n < 7) return (seq_t)0;

    /* Compute C(n,7) and multiply by 128 */
    seq_t res = binomial(n, (idx_t)7);
    res = res * (seq_t)128;   /* 2^7 = 128 */

    return res;
}
