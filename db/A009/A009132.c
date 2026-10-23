/*
 * A009132  Expansion of e.g.f. cosh(log(1+x))/exp(x).
 * https://oeis.org/A009132
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (-1)^n * (S_n - n + 1) / 2 where S_0 = 1 and S_n = n *
 *            S_{n-1} + 1
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 12, i64 n <= 20, i128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t S = (seq_t)1;          /* S_0 */
    for (idx_t i = 1; i <= n; ++i) {
        S = (seq_t)i * S + (seq_t)1;
    }
    seq_t val = (S - (seq_t)n + (seq_t)1) / (seq_t)2;
    if ((n & (idx_t)1) == (idx_t)0) {
        return val;
    } else {
        return -val;
    }
}
