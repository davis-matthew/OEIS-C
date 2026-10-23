/*
 * A138229  Expansion of (1-x)/(1-2x+6x^2).
 * https://oeis.org/A138229
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 2*a(n-1) - 6*a(n-2), a(0)=1, a(1)=1
 * Range:     i8 n <= 5, i16 n <= 12, i32 n <= 24, i64 n <= 48, i128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    seq_t a0 = (seq_t)1;
    seq_t a1 = (seq_t)1;
    for (idx_t i = 2; i <= n; ++i) {
        seq_t an = (seq_t)2 * a1 - (seq_t)6 * a0;
        a0 = a1;
        a1 = an;
    }
    return a1;
}
