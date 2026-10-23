/*
 * A145218  a(n) is the self-convolution series of the sum of 5th powers of
 *          the first n natural numbers.
 * https://oeis.org/A145218
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{k=1}^{n} (k^5) * ((n-k+1)^5)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 14, u64 n <= 114, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k)
    {
        /* compute k^5 */
        seq_t k5 = (seq_t)k;
        k5 *= k5;           /* k² */
        k5 *= k5;           /* k⁴ */
        k5 *= (seq_t)k;     /* k⁵ */

        /* compute (n-k+1)^5 */
        idx_t r = n - k + 1;
        seq_t r5 = (seq_t)r;
        r5 *= r5;           /* r² */
        r5 *= r5;           /* r⁴ */
        r5 *= (seq_t)r;     /* r⁵ */

        sum += k5 * r5;
    }
    return sum;
}
