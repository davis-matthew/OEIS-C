/*
 * A213827  a(n) = n^2*(n+1)*(3*n+1)/4.
 * https://oeis.org/A213827
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2*(n+1)*(3*n+1)/4
 * Range:     u8 n <= 3, u16 n <= 16, u32 n <= 274, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implemented with intermediate halving to avoid overflow:
   a(n) = (n*(n+1)/2) * (n*(3*n+1)/2) */
seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    seq_t term1 = (n_seq * ((seq_t)(n + 1))) / 2;
    seq_t term2 = (n_seq * ((seq_t)(3 * n_seq + 1))) / 2;
    return term1 * term2;
}
