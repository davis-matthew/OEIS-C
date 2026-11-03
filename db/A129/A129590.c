/*
 * A129590  Numbers k such that prime(k) + 2*(k-1)*prime(k-1) is prime.
 * https://oeis.org/A129590
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 27, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Validated: partial, pass, 61 terms through n = 61 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers k such that prime(k) + 2·(k−1)·prime(k−1) is prime.
   a(n) returns the n‑th such k (offset 1). */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"

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

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

#ifdef __cplusplus
#include <vector>
/* C++ implementation – use std::vector for automatic object handling */
static std::vector<seq_t> cache;
static idx_t last_k = 0;

/* Prepare the cache so that it contains at least `need` terms. */
static void ensure_cache(idx_t need)
{
    if (cache.size() >= (size_t)need)
        return;

    cache.reserve(need);
    while (cache.size() < (size_t)need)
    {
        ++last_k;
        seq_t pk = get_prime(last_k);
        seq_t expr = pk;
        if (last_k > 1)
        {
            expr += (seq_t)2 * ((seq_t)(last_k - 1)) * get_prime(last_k - 1);
        }
        if (isPrimeGeneric(expr))
            cache.push_back(last_k);
    }
}

/* Return the n‑th element of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;          /* undefined for n<1 */
    ensure_cache(n);
    return cache[(size_t)n - 1];
}
#else
/* C implementation – store results in a dynamically resized array. */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t last_k = 0;

/* Grow the cache to hold at least `need` terms. */
static void ensure_cache(idx_t need)
{
    if (cache_len >= (size_t)need)
        return;

    if (!cache)
    {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(*cache));
        if (!cache)
            abort();
    }
    else if (cache_len >= cache_cap)
    {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(*cache));
        if (!tmp)
            abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    while ((size_t)cache_len < (size_t)need)
    {
        ++last_k;
        seq_t pk = get_prime(last_k);
        seq_t expr = pk;
        if (last_k > 1)
        {
            expr += (seq_t)2 * ((seq_t)(last_k - 1)) * get_prime(last_k - 1);
        }
        if (isPrimeGeneric(expr))
        {
            cache[cache_len++] = (seq_t)last_k;
            if (cache_len >= cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(*cache));
                if (!tmp)
                    abort();
                cache = tmp;
                cache_cap = new_cap;
            }
        }
    }
}

/* Return the n‑th element of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;          /* undefined for n<1 */
    ensure_cache(n);
    return cache[(size_t)n - 1];
}
#endif
