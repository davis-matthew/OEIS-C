/*
 * A025486  Least k with A025485(k) = n.
 * https://oeis.org/A025486
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   recurrence for A025486 a(0) = 0, a(1) = 3, a(2) = 5, a(3) = 10
 *            a(n) = a(n‑1)^2 / 4 + 1 for n ≥ 4
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 7, u64 n <= 8, u128 n <= 9,
 *            big any n
 * Validated: partial, pass, 11 terms through n = 10 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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

/* static storage for memoisation */
static seq_t *cache = NULL;      /* array of computed values          */
static size_t cache_size = 0;    /* number of valid entries in cache   */
static size_t cache_cap  = 0;    /* allocated capacity of `cache`       */

/* resize the cache so that it can hold at least (n+1) elements.
   Elements that already exist are preserved. */
static void ensure_capacity(idx_t n)
{
    if ((size_t)n + 1 <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap << 1 : 4;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;

    cache = seq_realloc(cache, cache_cap, new_cap);
    cache_cap = new_cap;
}

/* Compute the value a(n) (OEIS A025486) for any n ≥ 0, in any order. */
seq_t a(idx_t n)
{
    ensure_capacity(n);

    for (size_t i = cache_size; i <= (size_t)n; ++i) {
        if (i == 0)
            cache[i] = (seq_t)0;
        else if (i == 1)
            cache[i] = (seq_t)3;
        else if (i == 2)
            cache[i] = (seq_t)5;
        else if (i == 3)
            cache[i] = (seq_t)10;
        else {
            seq_t prev = cache[i - 1];
            /* a(i) = a(i‑1)^2 / 4 + 1  */
            cache[i] = (prev * prev) / (seq_t)4 + (seq_t)1;
        }
    }

    cache_size = (size_t)n + 1;
    return cache[(size_t)n];
}
