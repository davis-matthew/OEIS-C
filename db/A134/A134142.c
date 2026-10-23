/*
 * A134142  List of quadruples: 2*(-4)^n, -3*(-4)^n, 2*(-4^n), 2*(-4)^n, n >=
 *          0.
 * https://oeis.org/A134142
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabf, easy, sign
 * Formula:   a(n) = -2*a(n-1)-2*a(n-2) for n > 1; a(0) = 2, a(1) = -3
 * Range:     i8 n <= 12, i16 n <= 28, i32 n <= 44, i64 n <= 44, i128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)(-3);

    seq_t a0 = (seq_t)2;        /* a(0) */
    seq_t a1 = (seq_t)(-3);     /* a(1) */
    seq_t an;

    for (idx_t i = 2; i <= n; ++i) {
        an = -((seq_t)2) * a1 - ((seq_t)2) * a0;
        a0 = a1;
        a1 = an;
    }

    return a1;
}
