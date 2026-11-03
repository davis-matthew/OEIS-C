/*
 * A072937  Least k such that prime(n) appears in factorization of k! + 1.
 * https://oeis.org/A072937
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = smallest k such that prime(n) divides k! + 1 (i.e. k! ≡
 *            –1 (mod prime(n))), n ≥ 2
 * Range:     u8 n <= 59, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Validated: partial, pass, 65 terms through n = 66 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* get_prime(idx) returns the idx‑th prime (1‑based)  */
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

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

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

/* dynamic array helpers for arbitrary‑precision seq_t */

static seq_t *cache = NULL;      /* cache[n] holds a(n) for n ≥ 0 */
static size_t cache_cap = 0;     /* allocated number of elements */
static idx_t  cache_upto = 1;    /* highest n for which a(n) is computed */

/* enlarge the cache up to at least n (n ≥ 2) */
static void ensure_cache(idx_t n)
{
    if (n < 2 || n <= cache_upto)
        return;

    /* size needed for indices 0 … n (inclusive) */
    size_t needed = (size_t)n + 1;

    if (cache == NULL) {
        cache = seq_alloc(needed);
    } else if (needed > cache_cap) {
        cache = seq_realloc(cache, cache_cap, needed);
    }
    cache_cap = needed;

    /* compute new terms */
    for (idx_t idx = cache_upto + 1; idx <= n; ++idx) {
        uint64_t p = get_prime(idx);     /* prime(n) */
        uint64_t f = 1;                  /* running product modulo p */
        uint64_t k;
        for (k = 1; k < p; ++k) {
            f = (f * k) % p;
            if (f == p - 1)
                break;
        }
        cache[idx] = (seq_t)k;
    }
    cache_upto = n;
}

/* public entry point required by the tester framework */
seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;   /* undefined for n < 2 */

    ensure_cache(n);
    return cache[n];
}
