/*
 * A047597  Numbers that are congruent to {0, 2, 3, 4, 5} mod 8.
 * https://oeis.org/A047597
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*floor((n-1)/5) + [0,2,3,4,5][(n-1)%5]
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Acceptable residue offsets for each 8‑block */
    static const seq_t offsets[5] = {0, 2, 3, 4, 5};

    /* Convert to zero‑based index */
    idx_t t = n - 1;
    /* Compute block number and position within that block */
    idx_t k = t / 5;   /* block index (0‑based) */
    idx_t r = t % 5;   /* position within block (0‑5) */

    /* Compute the term */
    return ((seq_t)8 * (seq_t)k) + offsets[r];
}
