/*
 * A004652  Expansion of x*(1+x^2+x^4)/((1-x)*(1-x^2)*(1-x^3)).
 * https://oeis.org/A004652
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = ceiling(n^2/4)
 * Range:     u8 n <= 15, u16 n <= 510, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    seq_t n_squared = n_seq * n_seq;
    return (n_squared + 3) / 4;
}
