/*
 * A032196  Number of necklaces with 11 black beads and n-11 white beads.
 * https://oeis.org/A032196
 *
 * Offset:    11    (a(11) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil(binomial(n, 11) / n) (for n ≥ 11)
 * Range:     u8 n <= 13, u16 n <= 16, u32 n <= 34, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* For indices below the offset, behaviour is irrelevant. */
    if (n < 11)
        return 0;

    /* Compute C(n,11) via the utility function. */
    seq_t comb = binomial(n, (idx_t)11);

    /* Ceil division by n: (comb + n - 1) / n. */
    seq_t nn = (seq_t)n;
    seq_t res = (comb + nn - 1) / nn;
    return res;
}
