/*
 * A049637  Congruent to 2, 3, 6, 8, 10 or 12 mod 13, but not equal to 3.
 * https://oeis.org/A049637
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sequence of integers congruent to 2, 3, 6, 8, 10, or 12
 *            (mod 13), excluding the single number 3
 * Range:     u8 n <= 116, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* k is the 0‑based index in the sequence that would include 3.  The only
       difference between this sequence and that full sequence is that the
       element at index 1 (the number 3) is omitted, shifting all later
       indices by +1. */
    idx_t k = n + (n == 0 ? 0 : 1);
    idx_t q = k / 6;   /* number of complete 13‑term blocks */
    idx_t r = k % 6;   /* position within the block (0..5) */

    /* Residues in ascending order that are allowed in each 13‑term block. */
    static const unsigned char residues[6] = {2, 3, 6, 8, 10, 12};

    seq_t base = ((seq_t)13) * (seq_t)q;
    return base + (seq_t)residues[(size_t)r];
}
