/*
 * A121443  Sum of divisors d of n which are odd and n/d is not divisible by
 *          3.
 * https://oeis.org/A121443
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = Σ_{d|n, d odd, n/d mod 3 ≠ 0} d
 * Range:     u8 n <= 214, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t sigma_of(seq_t x)
{
    seq_t result = 1;
    /* factor x (which is odd and not divisible by 3) */
    for (idx_t p = 3; p * p <= x; p += 2)
    {
        if (x % p == 0)
        {
            seq_t term = 1;
            seq_t sum  = 1;
            while (x % p == 0)
            {
                x /= p;
                term *= p;
                sum  += term;
            }
            result *= sum;
        }
    }
    if (x > 1)
        result *= (1 + x);
    return result;
}

seq_t a(idx_t n)
{
    seq_t temp = (seq_t)n;
    idx_t b = 0;

    /* count exponent of 3 in n */
    while (temp % 3 == 0)
    {
        temp /= 3;
        ++b;
    }

    /* remove all factors of 2 */
    while (temp % 2 == 0)
        temp /= 2;

    seq_t sigma_m = sigma_of(temp);

    seq_t pow3 = 1;
    for (idx_t i = 0; i < b; ++i)
        pow3 *= 3;

    return sigma_m * pow3;
}
