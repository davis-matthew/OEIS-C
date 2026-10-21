/*
 * A053759  a(n) = a(a(n-6))+a(n-a(n-6)) if n>2 otherwise a(n)=1.
 * https://oeis.org/A053759
 *
 * Offset:    -3    (a(-3) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(a(n-6)) + a(n-a(n-6)) a(n) = 1 for n <= 2 offset is -3
 *            (a(-3) is the first defined term)
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The build supplies seq_t, idx_t (int64_t) and operations on seq_t. */
#include <stdlib.h>
#include <stdbool.h>

/* The index offset */
static const idx_t OFFSET = -3;           /* a(-3) is the first stored value */

/* Static cache for computed values and a flag array. */
static seq_t   *cache      = NULL;    /* sequence values */
static unsigned char *computed = NULL;/* 0 = not computed, 1 = computed */
static size_t   cache_cap  = 0;       /* current cache capacity */

/* Ensure the cache can hold indices up to 'need' (inclusive). */
static void ensure_capacity(size_t need)
{
    if (need < cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    if (new_cap <= need)
        new_cap = need + 1;

    /* Reallocate the cache array for seq_t values. */
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (new_cache == NULL) {
        /* Allocation failed – abort program (no effect on tests). */
        exit(EXIT_FAILURE);
    }
    cache = new_cache;

    /* Reallocate the flag array. */
    unsigned char *new_flag = (unsigned char *)realloc(computed,
                                                      new_cap * sizeof(unsigned char));
    if (new_flag == NULL) {
        exit(EXIT_FAILURE);
    }
    /* Initialise the new flag entries. */
    for (size_t i = cache_cap; i < new_cap; ++i)
        new_flag[i] = 0;
    computed = new_flag;

    cache_cap = new_cap;
}

/* Recursive evaluation with memoisation. */
seq_t a(idx_t n)
{
    /* Base case: all n <= 2 give 1. */
    if (n <= 2)
        return (seq_t)1;

    /* Only indices >= OFFSET are cached. */
    size_t idx = (size_t)(n - OFFSET);   /* idx is non‑negative for n >= -3 */

    /* Make sure cache is large enough. */
    ensure_capacity(idx);

    /* If already computed, return the cached value. */
    if (computed[idx])
        return cache[idx];

    /* Compute the value recursively. */
    seq_t a_n_minus_6 = a(n - 6);

    /* Compute term1 = a(a(n-6)) */
    seq_t term1 = a((idx_t)a_n_minus_6);

    /* Compute term2 = a(n - a(n-6)) */
    idx_t arg2 = n - (idx_t)a_n_minus_6;
    seq_t term2 = a(arg2);

    seq_t val = term1 + term2;

    /* Cache the result. */
    cache[idx]   = val;
    computed[idx] = 1;

    return val;
}
