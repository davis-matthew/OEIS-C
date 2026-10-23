/*
 * A160344  Numerator of Hermite(n, 26/31).
 * https://oeis.org/A160344
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 31^n * Hermite(n, 26/31). The integer values satisfy the
 *            recurrence a(0) = 1, a(1) = 52, a(n+1) = 52 * a(n) - 1922 * n *
 *            a(n-1) for n ≥ 1
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 9, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)52;

    seq_t prev = (seq_t)1;      /* a(0) */
    seq_t cur  = (seq_t)52;     /* a(1) */

    for (idx_t i = 1; i < n; ++i) {
        /* Compute a(i+1) = 52 * a(i) - 1922 * i * a(i-1) */
        seq_t next = (seq_t)52 * cur - (seq_t)1922 * (seq_t)i * prev;
        prev = cur;
        cur  = next;
    }
    return cur;
}
