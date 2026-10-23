/*
 * A158958  Numerator of Hermite(n, 3/4).
 * https://oeis.org/A158958
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) - 3*a(n-1) + 8*(n-1)*a(n-2) = 0 with a(0)=1, a(1)=3
 * Range:     i8 n <= 3, i16 n <= 7, i32 n <= 11, i64 n <= 20, i128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;

    seq_t a0 = (seq_t)1;       /* a(n-2) */
    seq_t a1 = (seq_t)3;       /* a(n-1) */
    seq_t an = (seq_t)0;       /* a(n) */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t mult = (seq_t)8 * (seq_t)(i - 1) * a0;   /* 8*(i-1)*a(n-2) */
        an = (seq_t)3 * a1 - mult;                    /* recurrence */
        a0 = a1;
        a1 = an;
    }
    return an;
}
