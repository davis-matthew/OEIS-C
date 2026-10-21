/*
 * A077919  Expansion of (1-x)^(-1)/(1+2*x-2*x^2+2*x^3).
 * https://oeis.org/A077919
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 10, i32 n <= 20, i64 n <= 28, i128 n <= 28,
 *            big any n
 * Helpers:   mul_small, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=-1, a(2)=5, a(3)=-13,
   a(n) = -a(n-1)+4*a(n-2)-4*a(n-3)+2*a(n-4) for n>=4 */

#include <stddef.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"
#include "../utils/mul_small.h"

static seq_t *cache        = NULL;
static size_t cache_cap    = 0;
static size_t cache_filled = 0;

static void init_cache(void)
{
    if (cache != NULL)
        return;
    cache_cap    = 4;
    cache_filled = 4;
    cache        = seq_alloc(cache_cap);
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)-1;
    cache[2] = (seq_t)5;
    cache[3] = (seq_t)-13;
}

seq_t a(idx_t n)
{
    init_cache();

    size_t target = (size_t)n;
    if (target < cache_filled)
        return cache[target];

    if (target >= cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= target)
            new_cap <<= 1;
        cache = seq_realloc(cache, cache_cap, new_cap);
        cache_cap = new_cap;
    }

    for (size_t i = cache_filled; i <= target; ++i) {
        seq_t t1 = -cache[i - 1];
        seq_t t2 = mul_small(cache[i - 2], 4);
        seq_t t3 = mul_small(cache[i - 3], 4);
        seq_t t4 = mul_small(cache[i - 4], 2);
        cache[i] = t1 + t2 - t3 + t4;
    }

    cache_filled = target + 1;
    return cache[target];
}
