/*
 * A062067  a(1) = 1; a(n) is smallest square > a(n-1) such that a(n) + a(n-1)
 *          is a prime.
 * https://oeis.org/A062067
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 23, u32 n <= 35, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   ensure_capacity, floor_sqrt, is_prime_big, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(1)=1; for n>1, a(n)=smallest square > a(n‑1) such that a(n)+a(n‑1) is prime */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"
#include "../utils/floor_sqrt.h"
#include "../utils/ensure_capacity.h"
#include "../utils/seq_mul.h"

/* cache for previously computed terms */
static seq_t *cache = NULL;
static idx_t cache_len = 0;      /* number of terms stored (1‑based) */
static size_t cache_cap = 0;     /* allocated capacity */

/* ensure the cache is large enough to hold at least n elements */
static void ensure_cache(idx_t n)
{
    if (n > cache_cap)
        ensure_capacity((size_t)n, &cache_cap, (void **)&cache, sizeof(seq_t));
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;                /* undefined – callers never ask for it */

    /* initialisation on first call */
    if (cache == NULL) {
        ensure_capacity(1, &cache_cap, (void **)&cache, sizeof(seq_t));
        cache[0] = (seq_t)1;           /* a(1) = 1 */
        cache_len = 1;
    }

    if (n <= cache_len)
        return cache[n-1];

    /* extend cache up to the requested index */
    ensure_cache(n);

    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t prev = cache[i-2];          /* a(i‑1) */

        /* start from the next integer greater than sqrt(prev) */
        seq_t r = floor_sqrt(prev) + (seq_t)1;
        seq_t sq = seq_mul(r, r);

        /* find the first square such that prev + sq is prime */
        while (!is_prime_big(prev + sq)) {
            ++r;
            sq = seq_mul(r, r);
        }

        cache[i-1] = sq;
    }

    cache_len = n;
    return cache[n-1];
}
