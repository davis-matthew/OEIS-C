/*
 * A047490  Numbers that are congruent to {0, 1, 2, 3, 5, 7} mod 8.
 * https://oeis.org/A047490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*floor((n-1)/6) + [0,1,2,3,5,7][(n-1)%6]
 * Range:     u8 n <= 192, u16 n <= 9817, u32 n <= 9817, u64 n <= 9817,
 *            u128 n <= 9817, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* i.e., the sequence of all non‑negative integers congruent to 0,1,2,3,5 or 7 (mod 8). */
seq_t a(idx_t n)
{
    /* n >= 1 by definition of the OEIS offset */
    idx_t k = n - 1;                       /* zero‑based index */
    idx_t block = k / 6;                   /* number of full 6‑term blocks */
    idx_t pos   = k % 6;                   /* position inside the block */

    /* the allowed residues in one block of 8 consecutive integers */
    static const seq_t offset[6] = {0, 1, 2, 3, 5, 7};

    seq_t result = (seq_t)8 * (seq_t)block + offset[pos];
    return result;
}
