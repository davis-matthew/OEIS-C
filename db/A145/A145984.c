/*
 * A145984  Number of "universes" built from n entities according to the
 *          following rules: 1. Each of the entities can be an element or a
 *          set. 2. Sets are entities that do have another entity as an
 *          element. 3. There must exist an element. 4. Two sets are identical
 *          when they own the same elements.
 * https://oeis.org/A145984
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = variations(2^n-1, n-1) i.e. a(n) = (2^n-1)! / (2^n-n)!
 *            for n ≥ 0
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* handle trivial cases */
    if (n <= 1)
        return (seq_t)1;

    /* compute 2^n */
    seq_t two_pow = pow2(n);

    /* start = 2^n - (n-1)  (first factor in the product) */
    seq_t start = two_pow - (seq_t)(n - 1);

    seq_t result = (seq_t)1;

    /* multiply the n-1 consecutive factors: start, start+1, ..., start+(n-2) */
    for (idx_t i = 0; i + 1 < n; ++i)
    {
        seq_t term = start + (seq_t)i;
        result *= term;
    }

    return result;
}
