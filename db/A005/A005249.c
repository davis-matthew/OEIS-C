/*
 * A005249  Determinant of inverse Hilbert matrix.
 * https://oeis.org/A005249
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = Product_{k=0..n-1} (2k+1)*binomial(2k,k)^2, a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = (seq_t)1;
    seq_t b = (seq_t)1; /* binomial(2k,k), starting at k = 0 */

    for (idx_t k = 0; k < n; k++) {
        seq_t factor = ((seq_t)2 * (seq_t)k + (seq_t)1) * b * b;
        result = result * factor;
        /* binomial(2k+2,k+1) = binomial(2k,k) * 2*(2k+1) / (k+1) */
        b = b * ((seq_t)4 * (seq_t)k + (seq_t)2) / ((seq_t)k + (seq_t)1);
    }

    return result;
}
