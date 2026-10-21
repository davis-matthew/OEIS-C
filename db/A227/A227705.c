/*
 * A227705  The Wiener index of the nanostar dendrimer defined pictorially as
 *          G(n) in the Darafsheh et al. reference.
 * https://oeis.org/A227705
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^n*(1728*n - 1872) + 2340*2^n - 270
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 9, u64 n <= 24, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t pow2 = 1;
    seq_t pow4 = 1;
    for (idx_t i = 0; i < n; ++i) {
        pow2 *= (seq_t)2;
        pow4 *= (seq_t)4;
    }
    seq_t term1 = pow4 * ((seq_t)1728 * n - (seq_t)1872);
    seq_t term2 = pow2 * (seq_t)2340;
    return term1 + term2 - (seq_t)270;
}
