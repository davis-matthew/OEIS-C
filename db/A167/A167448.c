/*
 * A167448  Smallest number with n distinct positive cubic exponents in its
 *          prime factorization.
 * https://oeis.org/A167448
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Π_{k=1..n} prime(k)^{(n-k+1)^3} (offset 0)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 2, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Validated: standard, pass, 9 terms through n = 8 (2026-11-03)
 * Helpers:   ipow_idx, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Utilities provided by the framework */
#include <string.h>
#include <stddef.h>
#include "../utils/ipow_idx.h"
#include "../utils/mem.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
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

/* cache for already computed terms */
static seq_t *cache       = NULL;    /* array of a(i) */
static size_t cache_cap   = 0;       /* allocated size of cache */
static size_t cache_len   = 0;       /* number of terms stored in cache */

/* ensure that cache has at least new_cap elements */
static void reserve_cache(size_t new_cap)
{
    if (new_cap <= cache_cap)
        return;
    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    if (!tmp) {
        abort(); /* unrecoverable allocation failure */
    }
    cache     = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0) {
        if (cache_len == 0) {
            reserve_cache(1);
            cache[0] = (seq_t)1;
            cache_len = 1;
        }
        return cache[0];
    }

    /* ensure cache capacity up to n */
    reserve_cache((size_t)n + 1);

    /* compute missing terms up to n */
    if (cache_len == 0) {          /* first time */
        cache[0] = (seq_t)1;
        cache_len = 1;
    }
    for (idx_t m = (idx_t)cache_len; m <= n; ++m) {
        seq_t acc = (seq_t)1;                 /* product accumulator */

        for (idx_t k = 1; k <= m; ++k) {
            idx_t exp  = m - k + 1;           /* (n-k+1) */
            idx_t exp3 = exp;
            exp3 *= exp;                      /* exp^2 */
            exp3 *= exp;                      /* exp^3 */

            seq_t pow = ipow_idx((seq_t)get_prime(k), exp3);
            acc = seq_mul(acc, pow);
        }
        cache[(size_t)m] = acc;
    }
    cache_len = (size_t)n + 1;

    return cache[(size_t)n];
}
