/*
 * A125618  (Sum of the squares of the quadratic nonresidues of prime(n)) /
 *          prime(n).
 * https://oeis.org/A125618
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (sum of squares of quadratic nonresidues of prime(n)) /
 *            prime(n)
 * Range:     u8 n <= 13, u16 n <= 108, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 62 terms through n = 945 (2026-11-03)
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

static seq_t *cache = NULL;          /* cache of computed sequence values */
static size_t cache_cap = 0;         /* allocated size of cache */

/* ensure the cache can hold values up to index idx (zero‑based) */
static void _ensure_cache(idx_t idx)
{
    if ((size_t)idx < cache_cap)
        return;
    size_t new_cap = (size_t)idx + 1;
    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    if (!tmp) /* allocation failure – return zero, caller may handle it */
        return;
    /* initialize newly allocated entries to 0 (unknown) */
    mem_set(tmp + cache_cap, 0, (new_cap - cache_cap) * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* compute a(n) */
seq_t a(idx_t n)
{
    if (n < 4)
        return (seq_t)0;

    _ensure_cache(n - 1);            /* n is 1‑based; cache is 0‑based */

    if (cache[n - 1] != (seq_t)0)    /* already computed */
        return cache[n - 1];

    /* --------------------------------------------------------------------
       Compute value for the n-th prime.
       -------------------------------------------------------------------- */
    uint32_t p = get_prime(n);       /* prime number; fits in uint32_t for n≤10000 */

    /* total sum of squares 1² + 2² + … + (p‑1)² */
    uint64_t total = (uint64_t)(p - 1) * p * (2ULL * p - 1ULL) / 6ULL;

    /* sum of squares of quadratic residues (mod p) */
    uint64_t sum_res = 0;
    uint32_t half = (p - 1) / 2;
    for (uint32_t x = 1; x <= half; ++x) {
        uint32_t r = (uint32_t)((uint64_t)x * x % p);   /* 1 ≤ r ≤ p‑1 */
        sum_res += (uint64_t)r * r;                    /* r² fits in 64‑bit */
    }

    /* sum of squares of quadratic nonresidues */
    uint64_t sum_non = total - sum_res;

    /* result is always an integer */
    seq_t value = (seq_t)(sum_non / p);

    cache[n - 1] = value;    /* memoize */
    return value;
}
