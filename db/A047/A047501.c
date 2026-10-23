/*
 * A047501  Numbers that are congruent to {1, 3, 4, 5, 7} mod 8.
 * https://oeis.org/A047501
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*floor((n-1)/5) + {1,3,4,5,7}[(n-1) mod 5]
 * Range:     u8 n <= 56, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the number of complete 5‑term blocks and the position
       within the current block. */
    idx_t q = (n - 1) / 5;      /* block index (0‑based)           */
    idx_t r = (n - 1) % 5;      /* offset inside the block (0‑4)   */

    /* Offsets of the allowed residues modulo 8. */
    static const seq_t pattern[5] = {1, 3, 4, 5, 7};

    /* Build the result as base block value plus the residue. */
    return ((seq_t)q * 8ULL) + pattern[r];
}
