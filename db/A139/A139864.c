/*
 * A139864  Primes of the form 2x^2 + 2xy + 67y^2.
 * https://oeis.org/A139864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th prime that can be represented as * 2x² + 2xy +
 *            67y² (OEIS A139864). The function is * memoised and works for
 *            any index order
 * Range:     u8 n <= 9, u16 n <= 822, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

/* integer square root (64‑bit) */
static inline uint64_t isqrt_u64(uint64_t x)
{
    uint64_t r = 0, bit = 1ULL << 62;
    while (bit > x) bit >>= 2;
    while (bit) {
        if (x >= r + bit) {
            x -= r + bit;
            r = (r >> 1) + bit;
        } else
            r >>= 1;
        bit >>= 2;
    }
    return r;
}

/* true iff p can be written as 2x²+2xy+67y²  (p a prime, p>=3) */
static bool has_representation(uint64_t p)
{
    /* test y ≥ 1 (y = 0 gives p = 2 only, handled elsewhere) */
    for (uint64_t y = 1; ; ++y) {
        uint64_t y2  = y * y;
        uint64_t term = 133ULL * y2;           /* 133 = 2*67 */
        if (term >= 2ULL * p) break;           /* delta would be negative */
        uint64_t delta = 2ULL * p - term;
        uint64_t s = isqrt_u64(delta);
        if (s * s == delta && ((s - y) & 1ULL) == 0ULL)
            return true;
    }
    return false;
}

/* ------------------------------------------------------------- */
/* Sequence implementation                                       */

seq_t a(idx_t n)
{
    if (n == 0)                /* undefined: return 0 */
        return (seq_t)0;

    /* static cache of found primes */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static uint64_t next_candidate = 3;   /* primes start from 3 after 2 */

    /* initialise cache if first call */
    if (cache == NULL) {
        size_t cap = (size_t)(n > 200 ? n : 200);
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache_len = 0;
    } else if (cache_len < n) {          /* grow if needed */
        cache = (seq_t *)realloc(cache, (size_t)n * sizeof(seq_t));
    }

    /* set first term */
    if (cache_len == 0) {
        cache[0] = (seq_t)2;
        cache_len = 1;
        if (n == 1)
            return cache[0];
    }

    /* generate further terms until index n is reached */
    while (cache_len < n) {
        /* find next prime */
        while (!is_prime(next_candidate))
            ++next_candidate;

        uint64_t p = next_candidate;
        ++next_candidate;

        if (has_representation(p)) {
            cache[cache_len++] = (seq_t)p;
        }
    }

    return cache[n - 1];
}
