/*
 * A049077  a(n) = n / gcd(n, binomial(n, floor(n/2))).
 * https://oeis.org/A049077
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n / gcd(n, binomial(n, floor(n/2)))
 * Range:     u8 n <= 511, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Helper to compute the exponent of prime p in m!  (Legendre's formula) */
static inline idx_t v_factorial(idx_t m, idx_t p)
{
    idx_t r = 0;
    while (m)
    {
        m /= p;
        r += m;
    }
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /*  not used, offset 1 */
    idx_t k = n / 2;                       /* floor(n/2) */
    idx_t temp = n;

    seq_t g = 1;                           /* will hold the gcd */

    /* factor n by trial division up to sqrt(n) */
    for (idx_t p = 2; p * p <= temp; ++p)
    {
        if (temp % p == 0)
        {
            idx_t exp_n = 0;
            while (temp % p == 0)
            {
                temp /= p;
                ++exp_n;
            }

            /* exponent of p in C(n,k) */
            idx_t exp_binom = v_factorial(n, p) - v_factorial(k, p) - v_factorial(n - k, p);

            idx_t e = exp_n < exp_binom ? exp_n : exp_binom;   /* exponent in gcd */
            for (idx_t i = 0; i < e; ++i)
                g *= (seq_t)p;
        }
    }

    /* remaining prime factor > sqrt(n) */
    if (temp > 1)
    {
        idx_t p = temp;
        idx_t exp_n = 1;                           /* prime appears once */

        idx_t exp_binom = v_factorial(n, p) - v_factorial(k, p) - v_factorial(n - k, p);
        idx_t e = exp_n < exp_binom ? exp_n : exp_binom;

        for (idx_t i = 0; i < e; ++i)
            g *= (seq_t)p;
    }

    return ((seq_t)n) / g;
}
