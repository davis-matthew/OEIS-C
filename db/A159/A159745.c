/*
 * A159745  Numerator of Hermite(n, 8/21).
 * https://oeis.org/A159745
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 16*a(n-1) - 882*(n-1)*a(n-2), a(0)=1, a(1)=16
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 10, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;          /* a(0) */
    if (n == 1)
        return 16;         /* a(1) */

    /* iterate using the recurrence */
    seq_t a0 = 1;          /* a(n-2) */
    seq_t a1 = 16;         /* a(n-1) */
    seq_t a2;              /* a(n) */

    for (idx_t i = 2; i <= n; ++i) {
        /* a2 = 16*a1 - 882*(i-1)*a0 */
        seq_t t1 = (seq_t)16 * a1;
        seq_t t2 = a0 * (seq_t)(i - 1);
        t2 *= (seq_t)882;
        a2 = t1 - t2;

        /* shift for next iteration */
        a0 = a1;
        a1 = a2;
    }
    return a1;            /* a(n) */
}
