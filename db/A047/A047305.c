/*
 * A047305  Numbers that are congruent to {2, 3, 4, 5, 6} mod 7.
 * https://oeis.org/A047305
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A047305: Numbers congruent to 2,3,4,5,6 modulo 7
   Formula: a(n) = 7·⌊(n‑1)/5⌋ + ((n‑1) mod 5) + 2
   Offset: 1  (a(1) = 2)                                  */

seq_t a(idx_t n)
{
    /* Index n is 1‑based; ignore n == 0 if it occurs. */
    idx_t n0 = n - 1;                    /* zero‑based count */
    idx_t block  = n0 / 5;               /* whole blocks of 5 selected numbers */
    idx_t offset = n0 % 5;               /* position inside the block (0..4) */

    seq_t base = (seq_t)block * (seq_t)7;        /* start of the current block */
    return base + (seq_t)(offset + 2);           /* add residue 2‑6 */
}
