/*
 * A159030  Numerator of Hermite(n, 1/9).
 * https://oeis.org/A159030
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 2*a(n-1) - 162*(n-1)*a(n-2), a(0)=1, a(1)=2
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 7, i64 n <= 13, i128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)1;
    if (n == (idx_t)1) return (seq_t)2;

    seq_t a0 = (seq_t)1;  /* a(n-2) */
    seq_t a1 = (seq_t)2;  /* a(n-1) */

    for (idx_t i = (idx_t)2; i <= n; ++i)
    {
        seq_t term = (seq_t)2 * a1;                     /* 2*a(n-1)   */
        seq_t coeff = ((seq_t)162 * (seq_t)(i - (idx_t)1)); /* 162*(n-1)  */
        seq_t prod  = coeff * a0;                       /* 162*(n-1)*a(n-2) */
        seq_t new_val = term - prod;                    /* recurrence  */
        a0 = a1;
        a1 = new_val;
    }
    return a1;
}
