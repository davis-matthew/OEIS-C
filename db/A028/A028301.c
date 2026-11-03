/*
 * A028301  a(n) = a(n-1)^2 + prime(n-1), with a(1) = 1.
 * https://oeis.org/A028301
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Validated: standard, pass, 12 terms through n = 12 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = a(n-1)^2 + prime(n-1),  a(1) = 1 */

#include <stdlib.h>          /* malloc, free, size_t */
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

/* Static cache for memoisation */
static seq_t *cache = NULL;   /* pointer to allocated array */
static size_t cache_cap = 0;  /* current capacity of cache */
static size_t cache_len = 0;  /* number of terms actually stored */

/* Helper: extend cache to at least 'need' elements */
static void ensure_cache(size_t need)
{
    if (need <= cache_cap)
        return;                 /* already sufficient */

    seq_t *newp = mem_new(seq_t, need);
    if (!newp) {
        /* allocation failure – abort (no exception handling in C) */
        abort();
    }

    /* copy old data, if any */
    if (cache) {
        for (size_t i = 0; i < cache_len; ++i)
            newp[i] = cache[i];
        mem_free(cache);
    }

    cache = newp;
    cache_cap = need;
}

seq_t a(idx_t n)
{
    if (n == 0)          /* offset starts at 1 – return 0 for safety */
        return (seq_t)0;

    /* initialise base term a(1) if cache empty */
    if (cache_len == 0) {
        ensure_cache(1);
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    if ((size_t)n <= cache_len)      /* already computed */
        return cache[(size_t)n - 1];

    /* extend cache to contain terms up to n */
    ensure_cache((size_t)n);

    /* compute missing terms */
    for (idx_t i = (idx_t)(cache_len + 1); i <= n; ++i) {
        seq_t prev = cache[(size_t)i - 2];                 /* a(i-1) */
        seq_t sq   = prev * prev;                          /* a(i-1)^2   */
        seq_t pr   = (seq_t)get_prime((idx_t)i - 1);       /* prime(i-1) */
        cache[(size_t)i - 1] = sq + pr;                    /* a(i)       */
    }

    cache_len = (size_t)n;
    return cache[(size_t)n - 1];
}
