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
 * Validated: standard, pass, 97 terms through n = 9445 (2026-11-01)
 * Helpers:   mulmod_seq, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mulmod_seq.h"
#include "../utils/powmod.h"

static int legendre_19_over_prime(uint64_t p)
{
    /* p is an odd prime > 2, p != 19 */
    seq_t val = powmod(19, (p - 1) / 2, p);
    if (val == 1)
        return 1;
    if (val == (seq_t)(p - 1))
        return -1;
    return 0;
}

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
