/*
 * A067432  Number of ways to represent the n-th prime in form p*q+p+q, where
 *          p and q are primes (see A066938).
 * https://oeis.org/A067432
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered pairs of primes (p,q) * such that the
 *            n‑th prime equals p*q + p + q. * * Equivalent to counting pairs
 *            of primes (p,q) with * (p+1)(q+1) = prime(n)+1, where p,q ≥ 2
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 106 terms through n = 9737 (2026-11-01)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include <stdlib.h>
#include "../utils/is_prime.h"

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
    /* Undefined for n==0; return 0 as a safe default. */
    if (n == 0) return 0;

    /* Retrieve the n‑th prime (1‑based). */
    uint32_t prime32 = get_prime(n);
    uint64_t prime = (uint64_t)prime32;

    /* Work with m = prime + 1. */
    uint64_t m = prime + 1;

    uint64_t count = 0;

    /* Enumerate divisors a of m with a ≥ 3 and a ≤ √m. */
    for (uint64_t a = 3; a * a <= m; ++a) {
        if (m % a != 0) continue;

        uint64_t b = m / a;
        if (b < 3) continue;          /* b-1 must be prime ≥ 2 */

        /* Check primality of p = a-1 and q = b-1. */
        if (is_prime((idx_t)(a - 1)) && is_prime((idx_t)(b - 1))) {
            ++count;
        }
    }

    /* Return the count as seq_t. */
    return (seq_t)count;
}
