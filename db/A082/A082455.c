/*
 * A082455  a(n) = prime(n) + prime(n-1) - a(n-1), a(0) = 0, a(1) = 4.
 * https://oeis.org/A082455
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 59 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A082455
   Formula:  a(0) = 0
             a(1) = 4
             a(n) = prime(n) + prime(n-1) – a(n-1)     for n ≥ 2
   The n‑th prime (prime(1)=2, prime(2)=3, …) is obtained from the
   supplied helper get_prime(idx_t); it is cast to seq_t when used.
*/
#include <stdlib.h>
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

/* ------------------------------------------------------------ */
/*  Cache for previously computed terms                          */
/* ------------------------------------------------------------ */
static seq_t *cache = NULL;     /* cache[0] = 0, cache[1] = 4, … */
static size_t cache_cap = 0;    /* allocated capacity of cache array  */
static size_t cache_len = 0;    /* number of terms currently stored  */

/* Ensure cache can address index n (n >= 0).               */
static void
ensure_cache(idx_t n)
{
    if (cache_cap > (size_t)n)
        return;

    size_t new_cap = cache_cap == 0 ? 16 : cache_cap;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;

    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    if (!tmp)
        return;          /* abort on out‑of‑memory – caller will get 0 */
    cache = tmp;
    cache_cap = new_cap;
}

/* ------------------------------------------------------------ */
/*   Main public routine                                        */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    if (n == 1)
        return 4;

    ensure_cache(n);

    /* Initialise the first two terms if the cache is empty. */
    if (cache_len < 2) {
        cache[0] = 0;
        cache[1] = 4;
        cache_len = 2;
    }

    /* Compute any missing values up to n. */
    if (cache_len <= (size_t)n) {
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t pi   = (seq_t)get_prime(i);     /* prime(i)  */
            seq_t pim1 = (seq_t)get_prime(i-1);   /* prime(i-1) */
            cache[i] = pi + pim1 - cache[i-1];
        }
        cache_len = n + 1;
    }

    return cache[n];
}
