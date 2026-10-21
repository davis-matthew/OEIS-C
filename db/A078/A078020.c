/*
 * A078020  Expansion of (1-x)/(1-x+2*x^2).
 * https://oeis.org/A078020
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n-1) - 2*a(n-2), a(0)=1, a(1)=0
 * Range:     i8 n <= 14, i16 n <= 30, i32 n <= 61, i64 n <= 126,
 *            i128 n <= 253, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    seq_t prevprev = (seq_t)1;  /* a(0) */
    seq_t prev     = (seq_t)0;  /* a(1) */
    seq_t cur;
    for (idx_t i = 2; i <= n; ++i) {
        cur = prev - (seq_t)2 * prevprev;
        prevprev = prev;
        prev = cur;
    }
    return prev;
}
