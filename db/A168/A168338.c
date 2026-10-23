/*
 * A168338  Sum of the largest digit of the divisors of n.
 * https://oeis.org/A168338
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = sum of the largest decimal digit of each divisor of n
 * Range:     u8 n <= 3779, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t largest_digit(idx_t v)
{
    seq_t mx = 0;
    while (v > 0)
    {
        seq_t d = v % 10;
        if (d > mx)
            mx = d;
        v /= 10;
    }
    /* v is never 0 for an actual divisor, but if it were, the largest
       digit would be 0, so we keep mx as 0 in that unlikely case. */
    return mx;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 1; i <= n / i; ++i)
    {
        if (n % i == 0)
        {
            seq_t ld1 = largest_digit(i);
            idx_t j = n / i;
            if (j == i)
                sum += ld1;
            else
                sum += ld1 + largest_digit(j);
        }
    }
    return sum;
}
