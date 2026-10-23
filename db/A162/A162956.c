/*
 * A162956  a(0) = 0, a(1) = 1; a(2^i + j) = 3a(j) + a(j + 1) for 0 <= j <
 *          2^i.
 * https://oeis.org/A162956
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 1018, u32 n <= 9273, u64 n <= 9273,
 *            u128 n <= 9273, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0, a(1)=1; a(2^i + j)=3*a(j)+a(j+1) for 0 <= j < 2^i.
   The function uses memoised recursion with a dynamic array that works
   in both C and C++.  */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

/* Return the largest power of two <= n (n>0). */
static idx_t highest_power_of_two(idx_t n)
{
    idx_t pow2 = 1;
    while ((pow2 << 1) <= n) {
        pow2 <<= 1;
    }
    return pow2;
}

/* Cache for memoisation. */
static seq_t *cache = NULL;
static idx_t  cache_cap = 0;

/* Grow the cache to accommodate indices up to n. */
static void ensure_cache(idx_t n)
{
    if (n < cache_cap) return;

    idx_t new_cap = cache_cap ? cache_cap * 2ULL : 1ULL;
    if (new_cap <= n + 1ULL) new_cap = n + 1ULL;

#ifdef __cplusplus
    /* C++: allocate with new[] to invoke constructors. */
    seq_t *tmp = new seq_t[new_cap];
    /* Copy old elements. */
    for (idx_t i = 0; i < cache_cap; ++i) {
        tmp[i] = cache[i];
    }
    /* Initialise new elements to zero (default ctor may already do this). */
    for (idx_t i = cache_cap; i < new_cap; ++i) {
        tmp[i] = 0;
    }
    /* Destroy old array. */
    delete[] cache;
    cache = tmp;
#else
    /* C: realloc works for POD types. */
    seq_t *tmp = realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    /* Zero-extend the new part. */
    if (new_cap > cache_cap) {
        memset(tmp + cache_cap, 0, (new_cap - cache_cap) * sizeof(seq_t));
    }
    cache = tmp;
#endif
    cache_cap = new_cap;
}

/* Recursive helper with memoisation. */
static seq_t recurse(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    /* Use cached value if available. */
    if (cache[n] != (seq_t)0) return cache[n];

    idx_t pow2 = highest_power_of_two(n);   /* 2^i <= n < 2^(i+1) */
    idx_t j    = n - pow2;                  /* 0 <= j < 2^i */

    seq_t v1 = recurse(j);
    seq_t v2 = recurse(j + 1);
    seq_t res = (seq_t)3 * v1 + v2;

    cache[n] = res;
    return res;
}

/* Public function required by the build system. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    ensure_cache(n);
    if (cache[n] != (seq_t)0) return cache[n];
    return recurse(n);
}
