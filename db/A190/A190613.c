/*
 * A190613  a(n) = Sum_{k=1..n} (-1)^(n-floor(n/k)) * floor(n/k).
 * https://oeis.org/A190613
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, look
 * Formula:   a(n) = Sum_{k=1..n} (-1)^(n-floor(n/k)) * floor(n/k)
 * Range:     i8 n <= 46, i16 n <= 8398, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k)
    {
        idx_t q = n / k;          /* floor(n/k) */
        seq_t term = (seq_t)q;    /* positive value */
        if ((n - q) & 1)           /* odd exponent -> negative sign */
            term = -term;
        sum += term;
    }
    return sum;
}
