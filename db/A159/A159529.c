/*
 * A159529  Numerator of Hermite(n, 1/17).
 * https://oeis.org/A159529
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(0)=1, a(1)=2; for n>=2, a(n)=2*a(n-1)-578*(n-1)*a(n-2)
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 5, i64 n <= 11, i128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    /* iterate recurrence */
    seq_t a_nm2 = (seq_t)1;      /* a(n-2) */
    seq_t a_nm1 = (seq_t)2;      /* a(n-1) */
    seq_t a_n = 0;

    for (idx_t i = 2; i <= n; ++i) {
        seq_t mult = (seq_t)578 * (i - 1);   /* 578*(i-1) */
        a_n = 2 * a_nm1 - mult * a_nm2;      /* a(i) = 2*a(i-1) - 578*(i-1)*a(i-2) */
        a_nm2 = a_nm1;                       /* shift window */
        a_nm1 = a_n;
    }
    return a_n;
}
