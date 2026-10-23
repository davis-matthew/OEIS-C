/*
 * A102486  a(n) = 4*a(n-1) - 5*a(n-2).
 * https://oeis.org/A102486
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 4*a(n-1) - 5*a(n-2) with a(0)=2, a(1)=6
 * Range:     i8 n <= 5, i16 n <= 12, i32 n <= 26, i64 n <= 53, i128 n <= 108,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 2;
    if (n == 1) return 6;
    seq_t a0 = 2;   /* a(n-2) */
    seq_t a1 = 6;   /* a(n-1) */
    seq_t a2;
    for (idx_t i = 2; i <= n; ++i) {
        a2 = 4 * a1 - 5 * a0;  /* recurrence */
        a0 = a1;
        a1 = a2;
    }
    return a1;
}
