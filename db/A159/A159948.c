/*
 * A159948  Numerator of Hermite(n, 22/23).
 * https://oeis.org/A159948
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(0)=1, a(1)=44, for n≥2: a(n) = 44*a(n-1) - 1058*(n-1)*a(n-2)
 *            (i.e. a(n+1)=44*a(n)-1058*n*a(n-1))
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 10, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)44;

    seq_t a_nm2 = (seq_t)1;      /* a(n-2) */
    seq_t a_nm1 = (seq_t)44;     /* a(n-1) */
    for (idx_t i = 1; i < n; ++i) {
        /* compute a(i+1) from a(i) and a(i-1) */
        seq_t term = (seq_t)1058 * (seq_t)i * a_nm2;   /* 1058*i*a(n-1) */
        seq_t a_n   = 44 * a_nm1 - term;               /* a(n+1) */
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_nm1;
}
