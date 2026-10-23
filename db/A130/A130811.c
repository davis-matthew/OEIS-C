/*
 * A130811  If X_1,...,X_n is a partition of a 2n-set X into 2-blocks then
 *          a(n) is equal to the number of 5-subsets of X containing none of
 *          X_i, (i=1,...n).
 * https://oeis.org/A130811
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 32 * binomial(n,5) for n >= 5
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"   /* provides: seq_t binomial(idx_t a, idx_t b) */

seq_t a(idx_t n)
{
    /* The sequence is defined only for n >= 5. For smaller arguments
       we return 0, though such calls are not part of the OEIS offset.   */
    if (n < 5) return 0;

    seq_t val = binomial(n, 5);
    val = val * 32;
    return val;
}
