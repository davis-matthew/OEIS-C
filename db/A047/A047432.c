/*
 * A047432  Numbers that are congruent to {0, 1, 4, 5, 6} mod 8.
 * https://oeis.org/A047432
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*floor((n-1)/5) + [0,1,4,5,6][(n-1)%5]
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* n >= 1  (the offset of the sequence) */
    idx_t k = n - 1;                    /* zero‑based index */
    idx_t q = k / 5;                    /* number of full blocks of 5 accepted numbers */
    idx_t r = k % 5;                    /* position inside the current block */
    static const seq_t residues[5] = {0, 1, 4, 5, 6};
    seq_t base = (seq_t)q * (seq_t)8;   /* 8 * number_of_full_blocks */
    return base + residues[r];
}
