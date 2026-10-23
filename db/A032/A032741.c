/*
 * A032741  a(0) = 0; for n > 0, a(n) = number of proper divisors of n
 *          (divisors of n which are less than n).
 * https://oeis.org/A032741
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of proper divisors of n, i.e., tau(n) - 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)            /* a(0) = a(1) = 0 by definition */
        return 0;

    seq_t cnt = 0;
    for (idx_t i = 1; i <= n / i; ++i)   /* avoid overflow */
    {
        if (n % i == 0)
        {
            ++cnt;            /* divisor i */
            if (i != n / i)   /* paired divisor n/i if distinct */
                ++cnt;
        }
    }

    return cnt - 1;          /* exclude n itself */
}
