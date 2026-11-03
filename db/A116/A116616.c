/*
 * A116616  Values of n such that prime(2n+1) mod 12 = 7.
 * https://oeis.org/A116616
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th integer k such that prime(2k+1) mod 12 = 7
 * Range:     u8 n <= 80, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 77 terms through n = 929 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

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

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                 /* undefined for offset 0 */

    /* Static state for memoisation */
    static idx_t   next_candidate = 0;   /* next k to test                     */
    static idx_t   cache_len      = 0;   /* number of terms cached             */
    static size_t  cache_cap      = 0;   /* current allocation size in terms    */
    static seq_t * cache          = NULL;/* array of cached terms               */

    /* Allocate cache once if needed.  A generous capacity guarantees
       that we never need to resize for n up to at least 10 000.   */
    if (cache == NULL) {
        cache_cap = 200000;                            /* safe upper bound */
        cache     = seq_alloc(cache_cap);
    }

    /* Generate terms until we have the requested index. */
    while (cache_len < n) {
        idx_t k = next_candidate;

        /* prime(2k+1) – the (2k+1)-th prime */
        uint32_t p = get_prime(2 * k + 1);

        /* Test the condition p mod 12 == 7. */
        if (p % 12 == 7) {
            cache[cache_len++] = (seq_t)k;
        }

        ++next_candidate;  /* move to next k */
    }

    /* a(n) is the (n‑1)th element of the cached list */
    return cache[n - 1];
}
