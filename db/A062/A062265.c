/*
 * A062265  Row sums of signed triangle A062140 (generalized a=4 Laguerre).
 * https://oeis.org/A062265
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = 1, a(1) = 4, for n ≥ 2 a(n) = 2*(n+1)*a(n-1) −
 *            (n-1)*(n+3)*a(n-2)
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 12, i64 n <= 18, i128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;

    seq_t a_nm2 = (seq_t)1;   /* a(0)  */
    seq_t a_nm1 = (seq_t)4;   /* a(1)  */
    seq_t newval;

    for (idx_t k = 2; k <= n; ++k) {
        /* t1 = (k+1) * a(k-1) */
        seq_t t1 = a_nm1 * (seq_t)(k + 1);

        /* t2 = (k-1)*(k+3) * a(k-2) */
        seq_t t2 = a_nm2 * (seq_t)(k - 1) * (seq_t)(k + 3);

        /* a(k) = 2*t1 - t2 */
        newval = (seq_t)2 * t1 - t2;

        /* shift for next iteration */
        a_nm2 = a_nm1;
        a_nm1 = newval;
    }

    return a_nm1;
}
