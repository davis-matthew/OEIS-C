/*
 * A000542  Sum of 8th powers: 1^8 + 2^8 + ... + n^8.
 * https://oeis.org/A000542
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..n} i^8, a(0) = 0
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 14, u64 n <= 175, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t s = 0;
    idx_t i;
    for (i = 1; i <= n; i++) {
        seq_t x = (seq_t)i;
        seq_t x2 = x * x;
        seq_t x4 = x2 * x2;
        s += x4 * x4;
    }
    return s;
}
