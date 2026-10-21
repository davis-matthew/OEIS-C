/*
 * A071090  Sum of middle divisors of n.
 * https://oeis.org/A071090
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum of the divisors d of n that lie in the interval
 *            [sqrt(n/2), sqrt(2n)). That is, add each divisor d of n such
 *            that 2·d² ≥ n and d² < 2·n
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    /* enumerate divisors up to sqrt(n) */
    for (idx_t i = 1; i * i <= n; ++i)
    {
        if (n % i == 0)
        {
            seq_t d = i;
            /* lower bound: 2*d*d >= n   (d >= sqrt(n/2)) */
            /* upper bound: d*d < 2*n     (d < sqrt(2n))   */
            if ((seq_t)2 * d * d >= n && d * d < (seq_t)2 * n)
                sum += d;

            /* counterpart divisor */
            idx_t j = n / i;
            if (j != i)
            {
                seq_t d2 = j;
                if ((seq_t)2 * d2 * d2 >= n && d2 * d2 < (seq_t)2 * n)
                    sum += d2;
            }
        }
    }
    return sum;
}
