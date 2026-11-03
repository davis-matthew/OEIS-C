/*
 * A023522  Sum of exponents in prime-power factorization of p(n)*p(n-1) - 1.
 * https://oeis.org/A023522
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A001222(p(n)*p(n-1)-1)
 * Range:     u8 n <= 8, u16 n <= 55, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 95 terms through n = 992 (2026-11-01)
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sum of exponents in the prime-power factorization of p(n)*p(n-1)-1.
   p(k) is the k‑th prime.
   Uses get_prime(k) to obtain primes and bigomega(x) to sum exponents. */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/bigomega.h"

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
    if (n < 2)            /* undefined for n < 2; return 0 */
        return 0;

    /* obtain the n-th and (n-1)-th prime */
    uint64_t pn  = (uint64_t)get_prime(n);
    uint64_t pn1 = (uint64_t)get_prime(n - 1);

    /* compute p(n)*p(n-1)-1 in seq_t to avoid overflow if seq_t > 64 bits */
    seq_t val = (seq_t)pn * (seq_t)pn1 - 1;

    /* bigomega expects an idx_t argument; cast val safely (fits in 64 bits) */
    return bigomega((idx_t)val);
}
