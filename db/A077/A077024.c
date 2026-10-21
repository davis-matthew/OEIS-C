/*
 * A077024  a(n) = Sum_{k=1..n} floor(n/k + 1/2).
 * https://oeis.org/A077024
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} floor(n/k + 1/2) (rounding n/k to nearest
 *            integer)
 * Range:     u8 n <= 55, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k)
    {
        idx_t t = n / k;          /* integer part of n/k   */
        idx_t r = n % k;          /* remainder              */
        /* floor(n/k + 1/2) is t if 2*r < k, else t+1   */
        if ((r << 1) >= k)
            sum += (seq_t)(t + 1);
        else
            sum += (seq_t)t;
    }
    return sum;
}
