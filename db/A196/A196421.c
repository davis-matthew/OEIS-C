/*
 * A196421  a(n) = prime(n)*T(n), where T = A000217.
 * https://oeis.org/A196421
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n) * T(n), T(n) = n*(n+1)/2 (offset 1)
 * Range:     u8 n <= 5, u16 n <= 31, u32 n <= 1025, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 56 terms through n = 9486 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
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

/* allocate an array of seq_t of given length (may be 0) */
static seq_t *seq_alloc_array(size_t n)
{
    return mem_new(seq_t, n);
}

static void free_seq_array(seq_t *p)
{
    mem_free(p);
}

/* Compute the nth term of OEIS A196421:   a(n) = prime(n) * n*(n+1)/2   */
/* Indexing follows the OEIS offset: a(1) is the first term.          */
seq_t a(idx_t n)
{
    /* Undefined for n < 1; return 0 for safety (offset depends on implementation). */
    if (n == 0)
        return (seq_t)0;

    /* Memoisation storage for previously computed values. */
    static seq_t *cache   = NULL;   /* array indexed from 0..cap-1 */
    static idx_t   cap    = 0;      /* allocated capacity (number of slots) */
    static idx_t   last   = 0;      /* largest index computed so far */

    /* Grow the array if needed so that index n is addressable. */
    if (n >= cap) {
        idx_t new_cap = n + 1;            /* need indices 0..n inclusive */
        seq_t *new_cache = seq_alloc_array(new_cap);
        if (!new_cache)
            abort();                     /* allocation failure */

        /* Copy old values if any. */
        if (cache) {
            for (idx_t i = 0; i <= last; ++i)
                new_cache[i] = cache[i];
            free_seq_array(cache);
        }

        cache = new_cache;
        cap   = new_cap;
    }

    /* Compute any missing entries up to n. */
    if (n > last) {
        for (idx_t i = last + 1; i <= n; ++i) {
            seq_t p   = (seq_t)get_prime(i);              /* prime(i) */
            seq_t tri = (seq_t)i * ((seq_t)i + 1) / 2;    /* triangular number T(i) */
            cache[i]  = p * tri;                          /* product */
        }
        last = n;
    }

    return cache[n];
}
