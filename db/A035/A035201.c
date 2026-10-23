/*
 * A035201  Coefficients in expansion of Dirichlet series Product_p
 *          (1-(Kronecker(m,p)+1)*p^(-s)+Kronecker(m,p)*p^(-2s))^(-1) for m =
 *          19.
 * https://oeis.org/A035201
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Sum_{d|n} Kronecker(19,d) (multiplicative)
 * Range:     u8 n <= 4338, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   legendre_19_over_prime, mod_mul_128, mod_pow_128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/legendre_19_over_prime.h"
#include "../utils/mod_mul_128.h"
#include "../utils/mod_pow_128.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* not part of the OEIS offset */
    uint64_t rem = (uint64_t)n;
    seq_t result = 1;

    /* factor 2 */
    if ((rem & 1ULL) == 0)
    {
        unsigned e = 0;
        while ((rem & 1ULL) == 0)
        {
            ++e;
            rem >>= 1;          /* rem /= 2 */
        }
        seq_t local = (e & 1) ? 0 : 1;   /* (1+(-1)^e)/2 for (19,2) = -1 */
        result *= local;
    }

    /* odd prime factors */
    for (uint64_t p = 3; p * p <= rem; p += 2)
    {
        if (rem % p == 0)
        {
            unsigned e = 0;
            while (rem % p == 0)
            {
                ++e;
                rem /= p;
            }
            if (p == 19ULL)
                continue;       /* a(19^e) = 1, no change to result */
            int sym = legendre_19_over_prime(p);
            seq_t local;
            if (sym == -1)
                local = (e & 1) ? 0 : 1;       /* (1+(-1)^e)/2 */
            else
                local = (seq_t)(e + 1);         /* e+1 for (19,p)=1 */
            result *= local;
        }
    }

    if (rem > 1ULL)            /* remaining prime factor */
    {
        uint64_t p = rem;
        unsigned e = 1;
        if (p != 19ULL)
        {
            int sym = legendre_19_over_prime(p);
            seq_t local;
            if (sym == -1)
                local = (e & 1) ? 0 : 1;
            else
                local = (seq_t)(e + 1);
            result *= local;
        }
    }

    return result;
}
