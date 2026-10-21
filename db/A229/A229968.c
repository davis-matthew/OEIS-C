/*
 * A229968  Numbers not divisible by 3 or 11.
 * https://oeis.org/A229968
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 33 * floor((n-1)/20) + r, * where r is the (n-1) mod
 *            20-th smallest integer in [1,33] * that is not divisible by 3 or
 *            11. This yields the numbers * coprime to 33 (i.e., not divisible
 *            by 3 nor by 11). The * function works for any n ≥ 1 and does not
 *            rely on a static * table of the enumerated...
 * Range:     u8 n <= 154, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uintptr_t if needed */

seq_t a(idx_t n)
{
    /* Handle the trivial case n == 0 (outside the offset) */
    if (n == 0)
        return 0;

    /* Compute the block index and the position within the block */
    idx_t block_index = (n - 1) / 20;          /* floor((n-1)/20) */
    idx_t pos_in_block = (n - 1) % 20;         /* 0‑based index for the residue */

    /* Base value: 33 times the block index, cast to seq_t */
    seq_t base = (seq_t)block_index * (seq_t)33;

    /* Find the residue r (1 ≤ r ≤ 33) that is the (pos_in_block)-th
     * element of the set {1≤r≤33 : r%3≠0 and r%11≠0}.  We scan the
     * range 1…33; at most 33 iterations, which is negligible even for
     * many calls. */
    seq_t r = 0;
    idx_t found = 0;
    for (seq_t val = 1; val <= 33; ++val)
    {
        if ((val % 3 != (seq_t)0) && (val % 11 != (seq_t)0))
        {
            if (found == pos_in_block)
            {
                r = val;
                break;
            }
            ++found;
        }
    }

    return base + r;
}
