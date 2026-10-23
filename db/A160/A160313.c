/*
 * A160313  Numerator of Hermite(n, 15/31).
 * https://oeis.org/A160313
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 31^n * Hermite(n, 15/31). Recurrence: a(0)=1, a(1)=30,
 *            a(n+2)=30*a(n+1)-1922*(n+1)*a(n)
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 9, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (n == 1) return 30;
    seq_t a0 = 1;          /* a(i)   */
    seq_t a1 = 30;         /* a(i+1) */
    for (idx_t i = 0; i + 2 <= n; ++i) {
        seq_t next = 30 * a1 - 1922 * (seq_t)(i + 1) * a0;
        a0 = a1;
        a1 = next;
    }
    return a1;
}
