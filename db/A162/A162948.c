/*
 * A162948  Numbers with a sum of digits equal to their smallest prime factor.
 * https://oeis.org/A162948
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 302, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A162948 – numbers whose sum of decimal digits equals their
   smallest prime factor.  Computed by linear search with caching. */
#include <stdint.h>
#include <stdlib.h>

/* Compute the sum of decimal digits of a 64‑bit integer. */
static seq_t digit_sum_uint64(uint64_t x)
{
    seq_t s = 0;
    while (x > 0) {
        s += (seq_t)(x % 10);
        x /= 10;
    }
    return s;
}

/* Return the smallest prime factor of a 64‑bit integer. */
static uint64_t spf_uint64(uint64_t n)
{
    if (n < 2)
        return n;
    if (n % 2 == 0)
        return 2;
    for (uint64_t i = 3; i <= n / i; i += 2) {
        if (n % i == 0)
            return i;
    }
    return n;          /* n is prime */
}

seq_t a(idx_t n)
{
    /* Static cache of found sequence terms, stored as uint64_t. */
    static uint64_t *cache = NULL;
    static size_t   cache_len  = 0;
    static size_t   cache_cap  = 0;
    static uint64_t next_candidate = 2;

    /* Return cached value if already available. */
    if (n > 0 && (size_t)n <= cache_len)
        return (seq_t)cache[(size_t)n - 1];

    /* Allocate cache on first use. */
    if (!cache) {
        cache_cap = 16;
        cache = (uint64_t *)malloc(cache_cap * sizeof(uint64_t));
    }

    /* Extend cache until it contains at least n terms. */
    while (cache_len < (size_t)n) {
        uint64_t cand = next_candidate++;
        uint64_t spf  = spf_uint64(cand);
        seq_t    sum  = digit_sum_uint64(cand);

        if (sum == (seq_t)spf) {
            if (cache_len + 1 > cache_cap) {
                cache_cap = cache_cap * 2;
                cache = (uint64_t *)realloc(cache, cache_cap * sizeof(uint64_t));
            }
            cache[cache_len++] = cand;
        }
    }

    /* Return the n‑th term (1‑based). */
    return (seq_t)cache[(size_t)n - 1];
}
