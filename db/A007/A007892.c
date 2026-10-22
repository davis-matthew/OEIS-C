/*
 * A007892  A Kutz sequence.
 * https://oeis.org/A007892
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n - 3*floor(n/4))^2
 * Range:     u8 n <= 54, u16 n <= 1014, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t x = (seq_t)n - 3 * ((seq_t)n / 4);
    return x * x;
}
