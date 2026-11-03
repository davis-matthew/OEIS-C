/*
 * A228593  Products of 2 successive primorials.
 * https://oeis.org/A228593
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = primorial(n-1) * primorial(n) Recurrence: a(1) = 2 a(n) =
 *            a(n-1) * prime(n-1) * prime(n) for n ≥ 2 (primes are enumerated
 *            from 1: prime(1) = 2)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 16,
 *            big any n
 * Validated: standard, pass, 31 terms through n = 186 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
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

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

/* ensure the cache can hold at least (n+1) elements (index 1..n) */
static void ensure_capacity(idx_t n, seq_t **cache, size_t *cap) {
    size_t needed = (size_t)n + 1;
    if (*cap >= needed) return;
    /* grow (or allocate new) */
    if (*cache == NULL) {
        *cache = mem_new(seq_t, needed);
    } else {
        *cache = mem_resize(seq_t, *cache, needed);
    }
    if (*cache == NULL) abort();      /* memory exhaustion */
    *cap = needed;
}

seq_t a(idx_t n) {
    /* a(1) is the first term, behavior below 1 does not matter */
    if (n == 0) return 0;

    /* static cache for previously computed values */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static idx_t  max_n = 0;

    /* make room for index n */
    ensure_capacity(n, &cache, &cap);

    /* initialise first term if cache is empty */
    if (max_n == 0) {
        cache[1] = (seq_t)2;          /* primorial(0)*primorial(1) = 1*2 */
        max_n   = 1;
    }

    /* compute missing values up to n */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        seq_t prev = cache[i-1];
        /* fetch successive primes */
        uint32_t p1 = get_prime(i-1);  /* prime(i-1) */
        uint32_t p2 = get_prime(i);    /* prime(i) */
        /* multiply */
        seq_t tmp = seq_mul((seq_t)p1, (seq_t)p2);
        cache[i] = seq_mul(prev, tmp);
    }
    max_n = n;

    return cache[n];
}
