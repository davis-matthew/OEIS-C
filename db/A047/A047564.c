/*
 * A047564  Numbers that are congruent to {1, 3, 4, 5, 6, 7} mod 8.
 * https://oeis.org/A047564
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*floor((n-1)/6) + offset[(n-1)%6], where offset =
 *            {1,3,4,5,6,7}
 * Range:     u8 n <= 192, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Returns the nth number congruent to {1,3,4,5,6,7} modulo 8 (a(1)=1). */
seq_t a(idx_t n)
{
    /* offsets for the six admissible residues within each block of 8 numbers. */
    static const seq_t offset[6] = {1, 3, 4, 5, 6, 7};

    /* Zero‑based index into the sequence */
    idx_t k = n - 1;

    /* Block number and position within the block */
    idx_t q = k / 6;           /* how many full blocks of 6 numbers before n */
    idx_t r = k % 6;           /* position inside the current block */

    /* Compute the base value of the block as a seq_t. */
    seq_t base = (seq_t)8 * (seq_t)q;

    /* Return the correct sequence value. */
    return base + offset[r];
}
