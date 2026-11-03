/*
 * A076927  a(n) = A076926(n)/n.
 * https://oeis.org/A076927
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = denominator of n * sum_{k=1}^n 1/prime(k)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Validated: partial, pass, 18 terms through n = 18 (2026-11-01)
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent integer formula: a(n) = product of the first n primes that do NOT divide n */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/build_primes_upto.h"

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
    /* Cached list of the first primes as needed */
    static uint64_t *prime_list = NULL;
    static size_t prime_count = 0;
    static idx_t cached_n = 0;

    /* Ensure we have the first n primes in prime_list */
    if (n > cached_n) {
        free(prime_list);               /* release old buffer */
        prime_list   = NULL;
        prime_count  = 0;
        cached_n     = n;

        /* The n-th prime is the upper bound for which the first n primes lie */
        idx_t limit = get_prime(n);

        /* Build all primes up to that limit */
        build_primes_upto(limit, &prime_list, &prime_count);
        /* prime_count should now be equal to n */
    }

    /* Compute product of primes that do NOT divide n */
    seq_t prod = (seq_t)1;
    for (idx_t i = 0; i < n; ++i) {
        uint64_t p = prime_list[i];
        if (n % p != 0) {
            prod *= (seq_t)p;
        }
    }
    return prod;
}
