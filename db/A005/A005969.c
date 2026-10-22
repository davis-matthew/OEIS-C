/*
 * A005969  Sum of fourth powers of Fibonacci numbers.
 * https://oeis.org/A005969
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..n} F(i)^4, where F(i) = A000045(i), F(0) = 0,
 *            F(1) = 1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 24, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t x = 0, y = 1, s = 0;
    idx_t i;
    for (i = 1; i <= n; i++) {
        seq_t sq = y * y;
        s += sq * sq;
        seq_t t = x + y;
        x = y;
        y = t;
    }
    return s;
}
