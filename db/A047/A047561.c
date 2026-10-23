/*
 * A047561  Numbers that are congruent to {1, 2, 3, 6, 7} mod 8.
 * https://oeis.org/A047561
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 160, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A047561: first positive integer congruent to 1, 2, 3, 6, or 7 modulo 8 */
seq_t a(idx_t n) {
    /* For n ≥ 1 let k = (n‑1)/5 and r = (n‑1)%5.
       The allowed residues in order are {1, 2, 3, 6, 7}.
       The n‑th term is 8*k + residues[r]. */
    static const seq_t residues[5] = {1, 2, 3, 6, 7};
    idx_t k = (n - 1) / 5;
    idx_t r = (n - 1) % 5;
    return ((seq_t)8 * k) + residues[r];
}
