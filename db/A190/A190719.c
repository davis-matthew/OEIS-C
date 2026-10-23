/*
 * A190719  Numbers that are congruent to {0, 1, 3, 5, 7, 8, 11} mod 12.
 * https://oeis.org/A190719
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Package: A190719
   Formula: a(n) = 12 * floor((n-1)/7) + residues[(n-1)%7]   (n >= 1)
   Residues are 0,1,3,5,7,8,11 modulo 12. */
static const seq_t residues[7] = {0, 1, 3, 5, 7, 8, 11};

seq_t a(idx_t n) {
    /* Offset 1: a(1) corresponds to the first term 0. */
    idx_t idx = n - 1;                /* zero–based index */
    idx_t block = idx / 7;             /* number of full 12‑period blocks */
    idx_t rem   = idx % 7;             /* position within the 7‑term block */
    seq_t val = (seq_t)12 * (seq_t)block + residues[rem];
    return val;
}
