/*
 * A186595  Number of (n+3) X 8 0..3 arrays with every 4 X 4 subblock
 *          commuting with each horizontal and vertical neighbor 4 X 4
 *          subblock.
 * https://oeis.org/A186595
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * Empirical recurrence: * a(n) = 2*a(n-1) - a(n-2) + 3*a(n-4) -
 *            6*a(n-5) + 3*a(n-6) * + 3*a(n-8) - 3*a(n-9) for n > 15 * Initial
 *            values (OEIS offset 1): * a(1)=21993587 a(2)=444066 a(3)=1398069
 *            a(4)=5080629 * a(5)=11494293 a(6)=12478581 a(7)=12563805
 *            a(8)=12653493 * a(9)=12809421 a(10)=13255797 a...
 * Range:     u8 none, u16 none, u32 n <= 24, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, abort */
#include <stddef.h>   /* size_t */

static seq_t *cache = NULL;            /* 1‑based cache[0..cache_len] */
static size_t cache_len = 0;           /* largest index stored */
static size_t cache_cap = 0;           /* allocated capacity */

static const seq_t init_vals[16] = {
    [0] = (seq_t)0,
    [1] = (seq_t)21993587,
    [2] = (seq_t)444066,
    [3] = (seq_t)1398069,
    [4] = (seq_t)5080629,
    [5] = (seq_t)11494293,
    [6] = (seq_t)12478581,
    [7] = (seq_t)12563805,
    [8] = (seq_t)12653493,
    [9] = (seq_t)12809421,
    [10] = (seq_t)13255797,
    [11] = (seq_t)15722997,
    [12] = (seq_t)19992861,
    [13] = (seq_t)36147621,
    [14] = (seq_t)56126589,
    [15] = (seq_t)101158704
};

/* Allocate or reallocate the cache, copying existing entries. */
static void grow_cache(size_t new_cap)
{
    if (new_cap <= cache_cap) return;

#if defined(__cplusplus)
    seq_t *newp = new seq_t[new_cap + 1];
    if (cache) {
        for (size_t i = 0; i <= cache_len; ++i) newp[i] = cache[i];
        delete[] cache;
    }
    cache = newp;
    cache_cap = new_cap;
#else
    seq_t *newp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
    if (!newp) abort();
    cache = newp;
    cache_cap = new_cap;
#endif
}

/* Ensure cache contains a(0)...a(n). */
static void ensure_cache(idx_t n)
{
    if (n <= 15) return;          /* first 15 terms are known */

    if (cache == NULL) {
        /* Start with enough space for indices 0..15 */
        grow_cache(15);
        for (size_t i = 0; i <= 15; ++i) cache[i] = init_vals[i];
        cache_len = 15;
    }

    if ((size_t)n <= cache_len) return;   /* already computed */

    /* Grow capacity if necessary */
    grow_cache((size_t)n);

    /* Compute missing terms from cache_len+1 up to n */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        size_t idx = (size_t)i;
        cache[idx] =
            (seq_t)2 * cache[idx-1]      /* 2*a(n-1) */
            - cache[idx-2]               /* -a(n-2) */
            + (seq_t)3 * cache[idx-4]    /* +3*a(n-4) */
            - (seq_t)6 * cache[idx-5]    /* -6*a(n-5) */
            + (seq_t)3 * cache[idx-6]    /* +3*a(n-6) */
            + (seq_t)3 * cache[idx-8]    /* +3*a(n-8) */
            - (seq_t)3 * cache[idx-9];   /* -3*a(n-9) */
    }
    cache_len = (size_t)n;
}

/* Main sequence function required by the build. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;             /* out of range, arbitrary */
    if (n <= 15) return init_vals[(size_t)n];/* direct lookup */
    ensure_cache(n);
    return cache[(size_t)n];
}
