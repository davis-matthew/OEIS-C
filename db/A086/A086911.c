/*
 * A086911  a(n) = abs(prime(n)-a(n-1)) + abs(prime(n)-a(n-2)).
 * https://oeis.org/A086911
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |prime(n)-a(n-1)| + |prime(n)-a(n-2)|, a(1)=1, a(2)=1
 * Range:     u8 n <= 54, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Validated: partial, pass, 62 terms through n = 62 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
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

static seq_t *cache = NULL;      /* 1‑based cache of computed a(n) */
static idx_t cache_cap = 0;      /* number of elements allocated (including index 0) */

static seq_t abs_diff(seq_t x, seq_t y) { return (x > y) ? (x - y) : (y - x); }

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1 || n == 2) return (seq_t)1;

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 3;                         /* positions 0,1,2 */
        cache = mem_new(seq_t, (size_t)cache_cap);
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
    }

    /* expand cache if necessary */
    if (n >= cache_cap) {
        size_t old_cap = (size_t)cache_cap;
        size_t new_cap = (size_t)n + 1;        /* 1‑based index, so +1 */
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        cache = tmp;
        cache_cap = (idx_t)new_cap;
        /* zero‑initialise new slots */
        for (size_t i = old_cap; i < new_cap; ++i) cache[i] = (seq_t)0;
    }

    /* compute terms up to n (in increasing order) */
    for (idx_t i = 3; i <= n; ++i) {
        seq_t prime_val = (seq_t)get_prime(i);
        seq_t d1 = abs_diff(prime_val, cache[i - 1]);
        seq_t d2 = abs_diff(prime_val, cache[i - 2]);
        cache[i] = d1 + d2;
    }

    return cache[n];
}
