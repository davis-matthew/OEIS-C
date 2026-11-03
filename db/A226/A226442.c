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
 * Validated: partial, pass, 48 terms through n = 48 (2026-11-01)
 * Helpers:   smallest_prime_factor
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
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/smallest_prime_factor.h"

/* Return the k‑th prime (k ≥ 1).  On first call the prime list
   is built incrementally using trial division.  The list is
   stored in a static array which is expanded as necessary.   */
static uint32_t get_prime(idx_t k)
{
    static uint32_t *pr = NULL;     /* array of primes */
    static size_t sz   = 0;         /* number of primes stored */
    static size_t cap  = 0;         /* allocated capacity */
    static uint32_t cand = 3;       /* candidate after 2 */

    /* initialise the prime table on first use */
    if (sz == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
        sz = 0;
    }

    /* grow the array until we have at least k primes */
    while (sz < (size_t)k) {
        if (sz == 0) {                /* add the first prime */
            pr[sz++] = 2;
            continue;
        }

        /* find the next prime candidate */
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }

        if (!ok) {                    /* not prime – keep searching */
            cand += 2;                /* only odd numbers remain */
            continue;
        }

        /* prime found – store it, grow table if needed */
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }

    return pr[(size_t)k - 1];
}

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
                         smallest_prime_factor((uint64_t)aComp);
        uint64_t spfB = (bComp <= 1) ? UINT64_MAX :
                         smallest_prime_factor((uint64_t)bComp);

        /* Smallest prime factor of the triangular number. */
        uint64_t spfT = (spfA < spfB) ? spfA : spfB;

        if (spfT == (uint64_t)p)
            return (seq_t)m;   /* found the desired index */
        ++m;
    }
}
