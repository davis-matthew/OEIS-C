/*
 * A178138  Apply partial sum operator 4 times to primes.
 * https://oeis.org/A178138
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{k=1}^n binomial(n-k+3,3) * p_k (apply the partial
 *            sum operator 4 times to the primes)
 * Range:     u8 n <= 5, u16 n <= 18, u32 n <= 161, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 53 terms through n = 9365 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
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

static seq_t *cache = NULL;       /* cache for computed terms */
static idx_t cache_size = 0;      /* allocated capacity of cache   */
static idx_t max_computed = 0;    /* largest n that has been computed */
static seq_t s1 = 0, s2 = 0, s3 = 0, s4 = 0;  /* running sums */

static void ensure_cache(idx_t n)
{
    if (n <= cache_size) return;
    idx_t new_cap = (cache_size == 0) ? 16 : cache_size * 2;
    while (new_cap < n) new_cap *= 2;
    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    if (!tmp) {
        /* In a serious implementation we might abort or handle the error. */
        abort();
    }
    cache = tmp;
    cache_size = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined, but harmless */

    /* Extend cache and compute terms up to n, if necessary */
    if (n > max_computed) {
        ensure_cache(n);
        for (idx_t k = max_computed + 1; k <= n; ++k) {
            seq_t prime_k = (seq_t)get_prime(k);
            s1 += prime_k;          /* first partial sum */
            s2 += s1;               /* second partial sum */
            s3 += s2;               /* third partial sum */
            s4 += s3;               /* fourth partial sum */
            cache[k - 1] = s4;      /* store a(k) */
        }
        max_computed = n;
    }
    return cache[n - 1];
}
