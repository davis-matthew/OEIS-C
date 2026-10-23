/*
 * A184109  a(n) = n + ceiling(n^2/9); complement of A184108.
 * https://oeis.org/A184109
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + ceil(n^2/9)
 * Range:     u8 n <= 15, u16 n <= 762, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;  /* undefined for offset zero, but safe for caller */
    seq_t sq = (seq_t)n * (seq_t)n;
    seq_t ceil_div = (sq + 8) / 9;
    return (seq_t)n + ceil_div;
}
