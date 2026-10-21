/*
 * A050462  a(n) = Sum_{d|n, n/d=1 mod 4} d^3.
 * https://oeis.org/A050462
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n, n/d = 1 (mod 4)} d^3
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1624, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          // Not used for offset 1
    seq_t total = 0;
    for (idx_t d = 1; d * d <= n; ++d)
    {
        if (n % d == 0)
        {
            idx_t k = n / d;              /* k = n/d */
            if (k % 4 == 1)
                total += (seq_t)d * (seq_t)d * (seq_t)d;   /* add d^3 */

            idx_t e = k; /* partner divisor e = n/d */
            if (e != d)                       /* avoid double-counting */
            {
                /* for e, condition is n/e = d % 4 == 1 */
                if (d % 4 == 1)
                    total += (seq_t)e * (seq_t)e * (seq_t)e;   /* add e^3 */
            }
        }
    }
    return total;
}
