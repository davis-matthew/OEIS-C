/*
 * A215945  a(n) = - 3^n*A(2*n+1), where A(n) = 3*A(n-1) + A(n-2) - A(n-3)/3,
 *          with A(0)=A(1)=3, A(2)=11.
 * https://oeis.org/A215945
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 33*a(n-1) - 27*a(n-2) + 3*a(n-3) a(0) = -3, a(1) = -105,
 *            a(2) = -3387
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 12, i128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)-3;
    if (n == 1) return (seq_t)-105;
    if (n == 2) return (seq_t)-3387;

    seq_t a0 = (seq_t)-3;    /* a(n-3)   */
    seq_t a1 = (seq_t)-105;  /* a(n-2)   */
    seq_t a2 = (seq_t)-3387; /* a(n-1)   */
    seq_t an = a2;

    for (idx_t i = 3; i <= n; ++i) {
        an = (seq_t)33 * a2 - (seq_t)27 * a1 + (seq_t)3 * a0;
        a0 = a1;
        a1 = a2;
        a2 = an;
    }
    return an;
}
