/*
 * A125180  a(n) = 2*a(n-1) + prime(n) - prime(n-1), a(1)=2, where prime(n)
 *          denotes the n-th prime.
 * https://oeis.org/A125180
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + prime(n) - prime(n-1), a(1)=2
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Validated: standard, pass, 47 terms through n = 931 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses an expanding static cache so that calls
   can be made in any order.  It depends on the utility functions
   alloc_seq, free_seq_array and get_prime, which are provided in
   the ../utils directory. */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Helper: allocate an array of seq_t and zero it. */
static seq_t *alloc_seq(size_t n)
{
    return mem_new(seq_t, n);
}

static void free_seq_array(seq_t *p)
{
    mem_free(p);
}

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
    static seq_t *cache = NULL;   /* cache[0] unused; cache[1..cache_len] valid */
    static idx_t cache_len = 0;   /* largest index computed so far */

    if (n == (idx_t)0)
        return (seq_t)0;          /* undefined for n=0, safe return */

    if (n > cache_len) {
        /* Allocate space for indices 0..n (index 0 unused) */
        size_t new_len = (size_t)n + 1;
        seq_t *new_cache = alloc_seq(new_len);
        if (!new_cache)
            abort();

        /* Copy existing values into the new cache */
        if (cache) {
            for (idx_t i = 1; i <= cache_len; ++i)
                new_cache[i] = cache[i];
        }

        /* Compute new values from cache_len+1 up to n */
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            if (i == (idx_t)1) {
                new_cache[1] = (seq_t)2;
            } else {
                seq_t prev = new_cache[i - 1];
                uint32_t pi  = get_prime(i);
                uint32_t pim = get_prime(i - 1);
                seq_t delta = (seq_t)(pi - pim);
                new_cache[i] = (seq_t)(prev * (seq_t)2 + delta);
            }
        }

        /* Free old cache and commit new one */
        if (cache)
            free_seq_array(cache);
        cache     = new_cache;
        cache_len = n;
    }

    return cache[n];
}
