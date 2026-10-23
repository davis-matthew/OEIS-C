/*
 * A101853  a(n) = n*(20 + 15*n + n^2)/6.
 * https://oeis.org/A101853
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*(20 + 15*n + n^2)/6
 * Range:     u8 n <= 3, u16 n <= 35, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t m = (seq_t)n;
    seq_t res = m * ((seq_t)20 + (seq_t)15 * m + m * m);
    return res / 6;
}
