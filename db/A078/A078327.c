/*
 * A078327  Numbers k such that A078142(k) = A006530(k).
 * https://oeis.org/A078327
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   numbers k such that A078142(k)=A006530(k)
 * Range:     u8 n <= 29, u16 n <= 277, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 72 terms through n = 988 (2026-11-01)
 * Helpers:   isqrt, largest_prime_factor, smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A078142(k) = Σ_{p|k, p prime} (ceil_sqrt(p)² – p) */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/isqrt.h"
#include "../utils/largest_prime_factor.h"
#include "../utils/smallest_prime_factor.h"

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

/* Test whether a given uint64_t k satisfies the defining condition.  */
static bool is_valid_num(uint64_t k)
{
    uint64_t tmp = k;
    seq_t sum = 0;

    while (tmp > 1) {
        uint64_t p = smallest_prime_factor(tmp);
        /* largest prime factor will be found at the end via gpf(k)  */

        /* Differences of distinct prime factor p and the next square larger than p */
        seq_t sp = (seq_t)(isqrt(p) + 1);
        seq_t diff = sp * sp - (seq_t)p;
        sum += diff;

        /* Remove all occurrences of p */
        while (tmp % p == 0)
            tmp /= p;
    }
    /* Compare sum with the largest prime factor of k */
    return sum == (seq_t)largest_prime_factor(k);
}

/* Cached sequence */
static seq_t *cache = NULL;
static idx_t  cache_cap = 0;
static idx_t  cache_len = 0;
static uint64_t last_checked = 1;

/* Ensure the cache has the requested capacity, resizing via seq_realloc. */
static void ensure_cache_capacity(idx_t need)
{
    if (cache_cap < need) {
        idx_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap < need)
            new_cap <<= 1;
        seq_t *tmp = seq_realloc(cache, (size_t)cache_cap, (size_t)new_cap);
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Append one value to the cache. */
static void cache_append(seq_t val)
{
    ensure_cache_capacity(cache_len + 1);
    cache[cache_len++] = val;
}

/* Public function: return the n-th term of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0)   /* safety, sequence starts at index 1 */
        return (seq_t)0;

    /* Return cached value if already computed. */
    if (n <= cache_len)
        return cache[n - 1];

    /* Search sequentially until we have n terms. */
    for (uint64_t k = last_checked + 1; ; ++k) {
        if (is_valid_num(k))
            cache_append((seq_t)k);

        if (cache_len >= n) {
            last_checked = k;
            break;
        }
    }

    return cache[n - 1];
}
