/*
 * A077919  Expansion of (1-x)^(-1)/(1+2*x-2*x^2+2*x^3).
 * https://oeis.org/A077919
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 10, i32 n <= 20, i64 n <= 28, i128 n <= 28,
 *            big any n
 * Validated: partial, pass, 29 terms through n = 28 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=-1, a(2)=5, a(3)=-13,
   a(n) = -a(n-1)+4*a(n-2)-4*a(n-3)+2*a(n-4) for n>=4 */

#include <stddef.h>
#include <stdlib.h>

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

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

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
