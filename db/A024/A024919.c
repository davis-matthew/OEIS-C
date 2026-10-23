/*
 * A024919  a(n) = Sum_{k=1..n} (-1)^k*k*floor(n/k).
 * https://oeis.org/A024919
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=1..n} (-1)^k * k * floor(n/k)
 * Range:     i8 n <= 86, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        idx_t q = n / k;                 /* floor(n/k)   */
        seq_t term = (seq_t)k * (seq_t)q; /* convert before multiply to avoid idx_t overflow */
        if (k & 1)
            sum -= term;                 /* (-1)^k == -1 for odd k */
        else
            sum += term;                 /* (-1)^k == +1 for even k */
    }
    return sum;
}
