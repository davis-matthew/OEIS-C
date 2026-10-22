/*
 * A000888  a(n) = (2*n)!^2 / ((n+1)!*n!^3).
 * https://oeis.org/A000888
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(2*n,n)^2/(n+1) = binomial(2*n,n) *
 *            (binomial(2*n,n)/(n+1))
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t c = 1;
    idx_t i;
    /* c = binomial(2*i, i), built up by c(i+1) = c(i) * 2*(2*i+1) / (i+1), always exact */
    for (i = 0; i < n; i++) {
        seq_t si = (seq_t)i;
        c = c * (2 * (2 * si + 1)) / (si + 1);
    }
    seq_t cat = c / ((seq_t)n + 1);
    return c * cat;
}
