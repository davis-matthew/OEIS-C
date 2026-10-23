/*
 * A159947  Numerator of Hermite(n, 21/23).
 * https://oeis.org/A159947
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 23^n * Hermite(n, 21/23)
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 10, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)42;

    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)42;  /* a(1) */
    idx_t i;

    /* Use the recurrence a_{n+1} = 42*a_n - 1058*n*a_{n-1} */
    for (i = 2; i <= n; ++i) {
        seq_t cur = 42 * a1 - 1058 * ((seq_t)(i - 1) * a0);
        a0 = a1;
        a1 = cur;
    }
    return a1;
}
