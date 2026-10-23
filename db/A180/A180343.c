/*
 * A180343  a(0)=-4; a(n+1) = 2*a(n) + period 4: repeat 6,1,2,5.
 * https://oeis.org/A180343
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, less
 * Formula:   a(0) = -4, a(n+1) = 2*a(n) + period[n % 4] (period = 6,1,2,5)
 * Range:     i8 n <= 10, i16 n <= 18, i32 n <= 34, i64 n <= 66, i128 n <= 130,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* base value for a(0) */
    if (n == 0)
        return (seq_t)-4;

    /* periodic forcing terms – 16‑bit constants are safe for any seq_t */
    static const seq_t period[4] = { (seq_t)6, (seq_t)1, (seq_t)2, (seq_t)5 };

    seq_t  res = (seq_t)-4; /* a(0) */
    idx_t  j   = 0;         /* index into period */

    /* Iteratively apply the recurrence n times */
    for (idx_t i = 0; i < n; ++i) {
        res = res + res;         /* 2 * res */
        res += period[j];        /* add the current period term */
        j = (j + 1) & 3;         /* advance j; j cycles 0..3 */
    }

    return res;
}
