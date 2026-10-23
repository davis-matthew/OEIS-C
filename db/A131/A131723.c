/*
 * A131723  a(2*n) = 1-n^2, a(2*n+1) = n*(n+1).
 * https://oeis.org/A131723
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(2*n) = -(n*(n+2)), a(2*n+1) = n*(n+1)
 * Range:     i8 n <= 20, i16 n <= 360, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index parts to seq_t before arithmetic */
    if ((n & 1U) == 0) {                /* even index */
        seq_t k = (seq_t)(n >> 1U);     /* k = n/2                 */
        return -(k * (k + (seq_t)2));
    } else {                             /* odd index */
        seq_t k = (seq_t)((n + 1U) >> 1U);   /* k = (n+1)/2           */
        return k * (k + (seq_t)1);
    }
}
