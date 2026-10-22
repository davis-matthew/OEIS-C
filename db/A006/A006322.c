/*
 * A006322  4-dimensional analog of centered polygonal numbers.
 * https://oeis.org/A006322
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(5*n^2 + 5*n + 2)/24
 * Range:     u8 n <= 5, u16 n <= 23, u32 n <= 225, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_s = (seq_t)n;
    seq_t prod_n = n_s * (n_s + 1) / 2;
    seq_t poly = 5*n_s*n_s + 5*n_s + 2;
    return prod_n * poly / 12;
}
