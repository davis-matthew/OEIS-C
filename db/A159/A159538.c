/*
 * A159538  Numerator of Hermite(n, 10/17).
 * https://oeis.org/A159538
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 20*a(n-1) - 578*(n-1)*a(n-2). Initial values: a(0) = 1,
 *            a(1) = 20
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 6, i64 n <= 11, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 20;

    seq_t a0 = 1;   /* a(n-2) */
    seq_t a1 = 20;  /* a(n-1) */

    for (idx_t i = 2; i <= n; ++i)
    {
        /* compute a(i) = 20*a1 - 578*(i-1)*a0 */
        seq_t temp = (seq_t)(i - 1) * a0;   /* (i-1) * a(n-2) */
        seq_t an   = 20 * a1 - 578 * temp;   /* 20*a(n-1) - 578*(i-1)*a(n-2) */

        a0 = a1;      /* shift for next iteration */
        a1 = an;
    }
    return a1;
}
