/*
 * A020985  The Rudin-Shapiro or Golay-Rudin-Shapiro sequence (coefficients of
 *          the Shapiro polynomials).
 * https://oeis.org/A020985
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, nice, easy
 * Formula:   not stated
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=a(1)=1; a(2n)=a(n); a(2n+1)=a(n)*(-1)^n */
seq_t a(idx_t n)
{
    seq_t sign = 1;            /* the value a(n) will be either +1 or -1 */
    while (n > 1)              /* consider all bits except the least one */
    {
        if (n & 1)             /* n is odd, write n = 2k+1 where k = n>>1 */
        {
            if ((n >> 1) & 1)  /* if k is odd, multiply sign by -1 */
                sign = -sign;
        }
        n >>= 1;               /* move to the parent index  floor(n/2) */
    }
    return sign;
}
