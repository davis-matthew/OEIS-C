/*
 * A001464  Expansion of e.g.f. exp(-x - (1/2)*x^2).
 * https://oeis.org/A001464
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -a(n-1) - (n-1)*a(n-2), a(0) = 1, a(1) = -1
 * Range:     i8 n <= 7, i16 n <= 13, i32 n <= 20, i64 n <= 34, i128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t prev = 1;   /* a(0) */
    seq_t cur = -1;   /* a(1) */
    idx_t i;
    if (n == 0) return prev;
    for (i = 2; i <= n; i++) {
        seq_t next = -cur - (seq_t)(i - 1) * prev;
        prev = cur;
        cur = next;
    }
    return cur;
}
