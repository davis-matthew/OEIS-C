/*
 * A049047  Number of distinct factorial numbers congruent to 1 (mod
 *          prime(n)).
 * https://oeis.org/A049047
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of m (1 ≤ m < p_n) such that m! ≡ 1 (mod p_n),
 *            where p_n is the nth prime. a(1) = 1. Indexing starts at 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 121 terms through n = 9864 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/mem.h"

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

static seq_t *cache = NULL;        /* cached values of a(k), 0‑based */
static idx_t cache_len = 0;          /* number of values currently cached */

seq_t a(idx_t n)
{
    if (n < 1)                          /* undefined for n < 1 */
        return 0;

    /* Grow the cache to hold values up to n, computing any missing ones */
    if (n > cache_len)
    {
        size_t need = (size_t)n;
        cache = mem_resize(seq_t, cache, need);
        if (!cache)                     /* out of memory: best we can do */
            return 0;

        for (idx_t i = cache_len; i < n; ++i) /* i is 0‑based index */
        {
            uint32_t p = (uint32_t)get_prime(i + 1);  /* p_n, 1‑based */
            seq_t  cnt = 0;
            uint32_t fact = 1;                      /* factorial modulo p */

            for (uint32_t k = 1; k < p; ++k)
            {
                fact = (uint32_t)(( (uint64_t)fact * k ) % p);
                if (fact == 1)
                    ++cnt;
            }
            cache[i] = cnt;
        }
        cache_len = n;
    }
    return cache[n - 1];
}
