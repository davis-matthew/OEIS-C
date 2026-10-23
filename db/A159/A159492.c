/*
 * A159492  Numerator of Hermite(n, 2/13).
 * https://oeis.org/A159492
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(0)=1, a(1)=4, a(n) = 4*a(n-1) - 338*(n-1)*a(n-2) for n >= 2
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 6, i64 n <= 11, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is derived from a(n)=13^n * Hermite(n, 2/13), using
   the standard Hermite recurrence with x = 2/13. */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)4;

    seq_t a_nm2 = (seq_t)1;          /* a(n-2)   */
    seq_t a_nm1 = (seq_t)4;          /* a(n-1)   */
    seq_t a_n   = a_nm1;             /* placeholder */

    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t prod = (seq_t)338 *
                     (seq_t)(i - 1) *
                     a_nm2;           /* 338*(i-1)*a(n-2) */
        a_n = 4 * a_nm1 - prod;     /* recurrence */
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
