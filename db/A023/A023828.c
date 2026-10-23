/*
 * A023828  Sum of exponents in prime-power factorization of C(4n,n-2).
 * https://oeis.org/A023828
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{p prime} (v_p((4n)!)-v_p((n-2)!)-v_p((3n+2)!)) i.e.
 *            the sum of exponents in the prime factorisation of C(4n,n-2)
 * Range:     u8 n <= 756, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Helper: exponent of prime p in m! using Legendre's formula. */
static seq_t vp_factorial(idx_t m, idx_t p)
{
    seq_t v = 0;
    while (m)
    {
        m /= p;
        v += m;
    }
    return v;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 2)                     /* defined only for n >= 2, but harmless */
        return 0;

    /* Limit for sieve: all primes needed are <= 4n. */
    idx_t limit = 4 * n;
    unsigned char *is_comp = (unsigned char *)calloc(limit + 1, 1);
    if (!is_comp)                 /* allocation failure */
        return 0;

    seq_t sum = 0;

    idx_t p, j;
    for (p = 2; p <= limit; ++p)
    {
        if (!is_comp[p])           /* p is prime */
        {
            seq_t e = vp_factorial(4 * n, p);
            e -= vp_factorial(n - 2, p);
            e -= vp_factorial(3 * n + 2, p);
            sum += e;

            if (p <= limit / p)
            {
                for (j = p * p; j <= limit; j += p)
                    is_comp[j] = 1;
            }
        }
    }

    free(is_comp);
    return sum;
}
