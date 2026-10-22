/*
 * A006484  a(n) = n*(n + 1)*(n^2 - 3*n + 5)/6.
 * https://oeis.org/A006484
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n + 1)*(n^2 - 3*n + 5)/6
 * Range:     u8 n <= 6, u16 n <= 25, u32 n <= 258, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_val = (seq_t)n;
    seq_t cubic = ((n_val - 2) * n_val + 2) * n_val + 5;
    return n_val * cubic / 6;
}
