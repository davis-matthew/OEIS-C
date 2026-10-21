/*
 * A068981  Arithmetic derivative of n*prime(n).
 * https://oeis.org/A068981
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + prime(n)*A003415(n)
 * Range:     u8 n <= 11, u16 n <= 79, u32 n <= 8191, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Where A003415(n) is the arithmetic derivative of n. */

#include "../utils/get_prime.h"

static seq_t arithmetic_derivative(idx_t n)
{
    seq_t d   = 0;
    seq_t ns  = (seq_t)n;
    idx_t t   = n;

    for (idx_t p = 2; p * p <= t; ++p)
    {
        if (t % p == 0)
        {
            unsigned e = 0;
            while (t % p == 0)
            {
                t /= p;
                ++e;
            }
            /* d += (n / p) * e */
            d += (ns / (seq_t)p) * (seq_t)e;
        }
    }
    if (t > 1)               /* remaining prime factor */
    {
        d += ns / (seq_t)t;  /* exponent is 1 */
    }
    return d;
}

seq_t a(idx_t n)
{
    seq_t pn = (seq_t)get_prime(n);          /* prime(n) */
    seq_t d  = arithmetic_derivative(n);    /* A003415(n) */
    return (seq_t)n + pn * d;
}
