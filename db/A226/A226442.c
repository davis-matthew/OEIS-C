/*
 * A226442  a(n) = smallest index m such that smallest prime factor of m-th
 *          triangular number is prime(n).
 * https://oeis.org/A226442
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 47, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   _smallest_prime_factor, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = smallest index m such that the smallest prime factor of the
   m‑th triangular number is prime(n).  T_m = m(m+1)/2.  This
   implementation searches m incrementally, computing the
   smallest prime factor of each triangular number by factoring its
   two coprime components.  It relies on the utility functions
   get_prime (to obtain the nth prime) and _smallest_prime_factor
   (to obtain the smallest prime factor of a uint64_t). */

#include <stdint.h>
#include <limits.h>
#include "../utils/get_prime.h"
#include "../utils/_smallest_prime_factor.h"

seq_t a(idx_t n)
{
    /* Obtain the target prime p = prime(n).  prime(1) = 2. */
    uint32_t p = get_prime(n);

    idx_t m = 1;
    while (1)
    {
        /* Split T_m into two coprime factors. */
        idx_t aComp, bComp;
        if (m & 1)   /* m is odd: T_m = ((m+1)/2) * m */
        {
            aComp = (m + 1) >> 1;  /* (m+1)/2 */
            bComp = m;
        }
        else        /* m is even: T_m = (m/2) * (m+1) */
        {
            aComp = m >> 1;       /* m/2 */
            bComp = m + 1;
        }

        /* Smallest prime factor of each component.  For component 1
           we treat the factor as "infinite" so that it never becomes
           the minimum. */
        uint64_t spfA = (aComp <= 1) ? UINT64_MAX :
                         _smallest_prime_factor((uint64_t)aComp);
        uint64_t spfB = (bComp <= 1) ? UINT64_MAX :
                         _smallest_prime_factor((uint64_t)bComp);

        /* Smallest prime factor of the triangular number. */
        uint64_t spfT = (spfA < spfB) ? spfA : spfB;

        if (spfT == (uint64_t)p)
            return (seq_t)m;   /* found the desired index */
        ++m;
    }
}
