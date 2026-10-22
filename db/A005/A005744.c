/*
 * A005744  Expansion of x*(1+x-x^2)/((1-x)^4*(1+x)).
 * https://oeis.org/A005744
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = (4*n^3 + 30*n^2 + 20*n - 3 + 3*(-1)^n) / 48
 * Range:     u8 n <= 2, u16 n <= 23, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    seq_t n2 = n_seq * n_seq;
    seq_t n3 = n2 * n_seq;
    seq_t base = 4*n3 + 30*n2 + 20*n_seq;
    if (n & 1) {
        return (base - 6) / 48;
    } else {
        return base / 48;
    }
}
