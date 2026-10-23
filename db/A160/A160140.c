/*
 * A160140  Numerator of Hermite(n, 13/27).
 * https://oeis.org/A160140
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 27^n * Hermite(n, 13/27) Recurrence derived from
 *            H_{n+1}=2x H_n-2n H_{n-1} with x=13/27: a(0)=1, a(1)=26 a(n+1) =
 *            26*a(n) - 1458*n*a(n-1) (n ≥ 1)
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 10, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;
    if (n == 1)
        return 26;

    seq_t a_prev2 = 1;      /* a(0) */
    seq_t a_prev1 = 26;     /* a(1) */
    seq_t a_cur;

    for (idx_t i = 1; i < n; ++i)   /* compute a(i+1) */
    {
        a_cur = 26 * a_prev1;
        a_cur -= 1458 * (seq_t)i * a_prev2;  /* i == current n-1 */
        a_prev2 = a_prev1;
        a_prev1 = a_cur;
    }
    return a_prev1;
}
