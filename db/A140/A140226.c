/*
 * A140226  Binomial transform of [1, 3, 3, 1, 1, -1, 1, -1, 1, ...].
 * https://oeis.org/A140226
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n^2 + 11)/3 for n >= 1, a(0) = 1
 * Range:     u8 n <= 8, u16 n <= 35, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    seq_t tn = (seq_t)n;
    seq_t val = (tn * ((tn * tn) + (seq_t)11)) / (seq_t)3;
    return val;
}
