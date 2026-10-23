/*
 * A158700  a(n) = Hermite(n, 18).
 * https://oeis.org/A158700
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 36*a(n-1) - 2*(n-1)*a(n-2), a(0)=1, a(1)=36
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 6, i64 n <= 12, i128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)36;
    seq_t prevprev = (seq_t)1;
    seq_t prev = (seq_t)36;
    seq_t cur;
    for (idx_t i = 2; i <= n; ++i) {
        cur = (seq_t)36 * prev - (seq_t)2 * ((seq_t)(i - 1)) * prevprev;
        prevprev = prev;
        prev = cur;
    }
    return prev;
}
