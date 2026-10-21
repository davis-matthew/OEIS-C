/*
 * A082909  a(n) = Sum_{d|n} (d mod 3).
 * https://oeis.org/A082909
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} (d mod 3)
 * Range:     u8 n <= 102, u16 n <= 102, u32 n <= 102, u64 n <= 102,
 *            u128 n <= 102, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    /* iterate over divisors up to sqrt(n) */
    for (idx_t d = 1; d <= n / d; ++d)
    {
        if (n % d == 0)
        {
            sum += (seq_t)(d % 3);
            idx_t e = n / d;
            if (e != d)
                sum += (seq_t)(e % 3);
        }
    }
    return sum;
}
