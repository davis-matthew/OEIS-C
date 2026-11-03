/*
 * A225225  Second smallest prime quadratic non-residue modulo the n-th prime.
 * https://oeis.org/A225225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = second smallest prime quadratic non-residue modulo
 *            prime(n)
 * Range:     u8 n <= 54, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Validated: partial, pass, 100 terms through n = 100 (2026-11-01)
 * Helpers:   is_prime, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "../utils/is_prime.h"
#include "../utils/powmod.h"

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
    /* Obtain the n-th prime (1‑indexed). */
    uint64_t p = get_prime(n);          /* returns a uint32_t, safely cast to uint64_t */

    /* For p = 2 or 3 the sequence specifies a(n) = 3. */
    if (p <= 3)
        return (seq_t)3;

    seq_t mod = (seq_t)p;               /* modulus for Legendre symbol */
    uint64_t exp = (p - 1) / 2;         /* exponent (p‑1)/2 for Legendre */
    int found = 0;

    for (uint64_t q = 2;; ++q)
    {
        if (!is_prime(q))
            continue;                   /* skip composite candidates */

        if (q == p)
            continue;                   /* reject the prime itself */

        /* Compute Legendre symbol (q | p) via Euler's criterion. */
        seq_t leg = powmod((seq_t)q, exp, mod);

        if (leg == (seq_t)p - 1)        /* non‑residue */
        {
            ++found;
            if (found == 2)
                return (seq_t)q;        /* second smallest non‑residue prime */
        }
    }
}
