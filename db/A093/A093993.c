/*
 * A093993  Number of divisors of n with a prime digital sum.
 * https://oeis.org/A093993
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = |{ d : d divides n and digit_sum(d) is prime }|
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static int is_prime_small(int s)
{
    if (s < 2)          return 0;
    if (s % 2 == 0)     return s == 2;
    for (int d = 3; d * d <= s; d += 2)
        if (s % d == 0) return 0;
    return 1;
}

seq_t a(idx_t n)
{
    seq_t count = 0;
    for (idx_t i = 1; i <= n / i; ++i)
    {
        if (n % i == 0)
        {
            /* divisor i */
            idx_t v = i;
            int sum = 0;
            while (v)
            {
                sum += v % 10;
                v /= 10;
            }
            if (is_prime_small(sum)) ++count;

            /* complementary divisor n/i */
            idx_t j = n / i;
            if (j != i)
            {
                v = j;
                sum = 0;
                while (v)
                {
                    sum += v % 10;
                    v /= 10;
                }
                if (is_prime_small(sum)) ++count;
            }
        }
    }
    return count;
}
