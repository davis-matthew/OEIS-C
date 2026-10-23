/*
 * A013647  Numbers k such that the period of the continued fraction for
 *          sqrt(k) contains no 1's.
 * https://oeis.org/A013647
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 73, u16 n <= 3592, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search implementation of A013647: numbers k such that the period of the
   continued fraction for sqrt(k) contains no 1's. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

static inline idx_t isqrt_u64(idx_t n)
{
    idx_t r = (idx_t)sqrt((long double)n);
    /* correct possible rounding errors */
    while ((unsigned __int128)(r + 1) * (r + 1) <= n) ++r;
    while ((unsigned __int128)r * r > n) --r;
    return r;
}

/* Return true iff the continued‑fraction period of sqrt(k) contains no 1. */
static bool has_no_one(idx_t k)
{
    idx_t a0 = isqrt_u64(k);
    if ((seq_t)a0 * (seq_t)a0 == k)          /* perfect square */
        return false;

    uint64_t m = 0, d = 1, a = a0;
    for (;;)
    {
        /* next terms of the algorithm */
        m = d * a - m;
        uint64_t m2 = (uint64_t)(((__int128)m) * m);
        d = (k - m2) / d;
        a = (a0 + m) / d;

        if (a == 2 * a0)                  /* period finished */
            break;
        if (a == 1)                       /* forbidden coefficient */
            return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                         /* undefined, but guard */

    /* static cache of found terms */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap   = 0;
    static idx_t  next_k = 2;             /* start checking from 2 */

    /* grow cache as needed */
    while (cache_size < n)
    {
        if (cache == NULL) {
            cache_cap = 64;
            cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        }

        /* find next qualifying k */
        while (!has_no_one(next_k))
            ++next_k;

        if (cache_size == cache_cap) {
            cache_cap *= 2;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }
        cache[cache_size++] = (seq_t)next_k;
        ++next_k;
    }
    return cache[n - 1];
}
