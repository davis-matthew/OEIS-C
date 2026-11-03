/*
 * A066940  Numbers k such that gcd(prime(k+1) + 1, prime(k) + 1) = 2.
 * https://oeis.org/A066940
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 84 terms through n = 953 (2026-11-03)
 * Helpers:   gcd, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence: A066940
 * Formula: k such that gcd(prime(k+1)+1, prime(k)+1) = 2
 * The term a(n) is the nth such k (offset 1).
 */
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/gcd.h"
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

/* --- static cache ------------------------------------------------------ */
static seq_t *cache = NULL;          /* array of found k values          */
static size_t  cache_len  = 0;       /* number of stored values          */
static size_t  cache_cap  = 0;       /* current allocation size          */
static idx_t   next_k_test = 1;      /* next k value to test             */

/* Grow the cache when full */
static void grow_cache(void)
{
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    if (!tmp) exit(EXIT_FAILURE);
    cache = tmp;
    cache_cap = new_cap;
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* guard for 0 --> offset is 1 */

    /* Return cached value if already computed */
    if ((size_t)n <= cache_len) return cache[(size_t)n - 1];

    /* Find subsequent terms until we have at least n */
    while (cache_len < (size_t)n) {
        uint32_t pk   = get_prime(next_k_test);       /* prime(k)   */
        uint32_t pk1p = get_prime(next_k_test + 1);   /* prime(k+1) */

        seq_t a1 = (seq_t)pk1p + (seq_t)1;           /* prime(k+1)+1 */
        seq_t a2 = (seq_t)pk   + (seq_t)1;           /* prime(k)+1   */
        seq_t g  = gcd(a1, a2);

        if (g == (seq_t)2) {                         /* condition holds */
            if (cache_len == cache_cap) grow_cache();
            cache[cache_len++] = next_k_test;
        }
        ++next_k_test;
    }

    return cache[(size_t)n - 1];
}
