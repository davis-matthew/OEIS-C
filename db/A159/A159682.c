/*
 * A159682  Numerator of Hermite(n, 19/20).
 * https://oeis.org/A159682
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 19*a(n-1) - 200*(n-1)*a(n-2); initial values: a(0)=1,
 *            a(1)=19
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 6, i64 n <= 12, i128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS A159682 */
seq_t a(idx_t n)
{
    /* handle the first two terms */
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)19;

    /* iterative recurrence using only the last two values */
    seq_t a_nm2 = (seq_t)1;   /* a(n-2), starts with a(0) */
    seq_t a_nm1 = (seq_t)19;  /* a(n-1), starts with a(1) */
    seq_t a_n;                /* current term */

    for (idx_t i = 2u; i <= n; ++i)
    {
        /* a(i) = 19*a(i-1) - 200*(i-1)*a(i-2) */
        a_n = (seq_t)19 * a_nm1
              - (seq_t)200 * (i - 1u) * a_nm2;
        a_nm2 = a_nm1;   /* shift window */
        a_nm1 = a_n;
    }

    return a_nm1;
}
