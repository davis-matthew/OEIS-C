/*
 * A092341  a(0)=1, a(n) = sigma_3(3n).
 * https://oeis.org/A092341
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sigma_3(3*n) (sum of cubes of divisors of 3*n)
 * Range:     u8 n <= 2, u16 n <= 13, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special case for n = 0 */
    if (n == 0)
        return (seq_t)1;

    /* Compute m = 3*n */
    idx_t m = 3 * n;
    seq_t sum = 0;

    /* Loop over divisors d up to sqrt(m) without overflow */
    for (idx_t d = 1; d <= m / d; ++d)
    {
        if (m % d == 0)
        {
            /* Add d^3 */
            seq_t d3 = (seq_t)d * d * d;
            sum += d3;

            idx_t e = m / d;
            if (e != d)
            {
                /* Add conjugate divisor e^3 */
                seq_t e3 = (seq_t)e * e * e;
                sum += e3;
            }
        }
    }
    return sum;
}
