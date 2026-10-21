/*
 * A227144  Numbers that are congruent to {1, 2, 7, 17, 23} modulo 24.
 * https://oeis.org/A227144
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 24 * floor((n - 1) / 5) + {1,2,7,17,23}[(n - 1) mod 5]
 * Range:     u8 n <= 53, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For n >= 1, returns the n-th integer congruent to 1, 2, 7, 17, or 23 modulo 24. */
seq_t a(idx_t n) {
    if (n == 0) return 0;                     /* undefined for offset, but return 0 */
    idx_t block = (n - 1) / 5;                /* which 24‑block */
    idx_t rem   = (n - 1) % 5;                /* position inside the block */
    static const seq_t residues[5] = {1, 2, 7, 17, 23};
    return (seq_t)24 * (seq_t)block + residues[rem];
}
