/*
 * A055142  Expansion of e.g.f.: exp(x)*sqrt(1-2x).
 * https://oeis.org/A055142
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 2*(n-1)*(a(n-1) - a(n-2)), a(0)=1, a(1)=0
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 11, i64 n <= 17, i128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the D‑finite recurrence a(n)+2*(1-n)a(n-1)+2*(n-1)a(n-2)=0 */
/* which is equivalent to a(n)=2*(n-1)*(a(n-1)-a(n-2)). */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;

    /* a(0) and a(1) */
    seq_t a_nm2 = (seq_t)1;  /* a(n-2) */
    seq_t a_nm1 = (seq_t)0;  /* a(n-1) */

    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t diff  = a_nm1 - a_nm2;
        seq_t mult  = (seq_t)2 * (seq_t)(i - 1);  /* 2*(n-1) as seq_t */
        seq_t ai    = diff * mult;

        a_nm2 = a_nm1;
        a_nm1 = ai;
    }
    return a_nm1;
}
