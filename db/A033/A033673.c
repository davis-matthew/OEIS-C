/*
 * A033673  Trajectory of 79 under map x->x + (x-with-digits-reversed).
 * https://oeis.org/A033673
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0) = 79, a(n) = a(n-1) + reverse_decimal(a(n-1)) for n > 0
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 17, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Validated: partial, pass, 22 terms through n = 21 (2026-11-01)
 * Helpers:   reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/reverse_digits.h"

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

/* static cache of computed terms */
static seq_t *cache = NULL;   /* array of seq_t */
static idx_t cache_max = 0;   /* largest index computed (valid after first init) */
static size_t cache_cap = 0;  /* capacity of cache array */

/* ensure cache contains terms up to index n */
static void ensure_cache(idx_t n)
{
    if (cache != NULL && n <= cache_max)
        return; /* already ready */

    if (cache == NULL) {
        /* first initialization: allocate space for n=0 term */
        cache_cap = 1;
        cache = seq_alloc(cache_cap);
        cache[0] = (seq_t)79;
        cache_max = 0;
        if (n == 0)
            return;
        n -= 0; /* continue to compute rest */
    }

    /* grow capacity if necessary */
    size_t needed = (size_t)(n + 1);
    if (needed > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap < needed)
            new_cap <<= 1;
        seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t prev = cache[(size_t)i - 1];
        seq_t rev  = reverse_digits(prev);
        cache[(size_t)i] = prev + rev;
    }
    cache_max = n;
}

/* sequence function */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
