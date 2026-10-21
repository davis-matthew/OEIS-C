/*
 * A094471  a(n) = Sum_{(n - k)|n, 0 <= k <= n} k.
 * https://oeis.org/A094471
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n} (n-d) = n*tau(n) - sigma(n)
 * Range:     u8 n <= 47, u16 n <= 2015, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where tau(n) = number of divisors of n, sigma(n) = sum of divisors */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined for n=0, but handle safely */
    seq_t sn = (seq_t)n;
    seq_t sigma = 1, tau = 1;
    idx_t temp = n;

    for (idx_t p = 2; p * p <= temp; ++p)
    {
        if (temp % p != 0) continue;
        idx_t e = 0;
        seq_t pow_p = 1;                  /* p^e */
        while (temp % p == 0)
        {
            temp /= p;
            ++e;
            pow_p *= (seq_t)p;            /* now pow_p = p^e */
        }

        /* sum of divisors for this prime factor: (p^(e+1)-1)/(p-1) */
        seq_t numerator = pow_p * (seq_t)p - 1;   /* p^(e+1) - 1 */
        seq_t denominator = (seq_t)p - 1;         /* p - 1 */
        seq_t term = numerator / denominator;     /* exact division */

        sigma *= term;
        tau *= (seq_t)(e + 1);
    }

    if (temp > 1)            /* remaining prime */
    {
        seq_t p = (seq_t)temp;
        sigma *= (1 + p);
        tau *= 2;
    }

    return sn * tau - sigma;
}
