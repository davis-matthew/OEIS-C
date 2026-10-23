/*
 * A035221  Coefficients in expansion of Dirichlet series Product_p
 *          (1-(Kronecker(m,p)+1)*p^(-s)+Kronecker(m,p)*p^(-2s))^(-1) for m =
 *          39.
 * https://oeis.org/A035221
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Sum_{d|n} Kronecker(39,d)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _jacobi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/_jacobi.h"

/*------------------------------------------------------------------
   Factor n into prime powers.
   pr[0..cnt-1]   : distinct prime factors
   ex[0..cnt-1]   : corresponding exponents
------------------------------------------------------------------*/
static void _factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt)
{
    size_t c = 0;
    for (uint64_t p = 2; p * p <= n; ++p)
    {
        if (n % p == 0)
        {
            uint64_t e = 0;
            while (n % p == 0)
            {
                n /= p;
                ++e;
            }
            pr[c] = p;
            ex[c] = e;
            ++c;
        }
    }
    if (n > 1)
    {
        pr[c] = n;
        ex[c] = 1;
        ++c;
    }
    *cnt = c;
}

/*------------------------------------------------------------------
   Main sequence function.
------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n = 0 */

    /* Factor n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt = 0;
    _factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;

    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        int kr;   /* Kronecker(39,p) */

        if (p == 2)
        {
            /* (39/2) = 1 because 39 ≡ 7 (mod 8) */
            kr = 1;
        }
        else if (p == 3 || p == 13)
        {
            /* 39 is divisible by p → symbol = 0 */
            kr = 0;
        }
        else
        {
            kr = _jacobi(39ULL, p); /* for odd p, Jacobi equals Kronecker */
        }

        seq_t contrib;
        if (kr == 0)
        {
            contrib = 1;
        }
        else if (kr == -1)
        {
            /* (1 + (-1)^e)/2 : 1 if e even, 0 if e odd */
            contrib = ((e & 1) == 0) ? 1 : 0;
        }
        else /* kr == 1 */
        {
            contrib = (seq_t)e + 1;
        }

        if (contrib == 0)
            return 0;  /* product becomes zero */

        result *= contrib;
    }

    return result;
}
