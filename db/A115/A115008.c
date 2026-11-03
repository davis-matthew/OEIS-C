/*
 * A115008  a(n) = a(n-1) + a(n-3) + a(n-4).
 * https://oeis.org/A115008
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 23, u32 n <= 46, u64 n <= 92, u128 n <= 185,
 *            big any n
 * Validated: standard, pass, 54 terms through n = 942 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=0, a(2)=2, a(3)=4;
   a(n) = a(n-1) + a(n-3) + a(n-4)  for n ≥ 4 */

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

static seq_t *cache = NULL;
static idx_t cache_size = 0;

/* Ensure that cached values exist up to index n. */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        /* Allocate space for the initial four terms. */
        idx_t init_cap = 4;
        cache = seq_alloc(init_cap);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)4;
        cache_size = init_cap;
    }

    if (n < cache_size)
        return;

    idx_t old_size = cache_size;
    idx_t new_size = n + 1;

    if (new_size > old_size) {
        cache = seq_realloc(cache, old_size, new_size);
        cache_size = new_size;
    }

    /* Compute terms from the first newly needed index up to n. */
    idx_t start = old_size > 4 ? old_size : 4;
    for (idx_t i = start; i <= n; ++i) {
        cache[i] = cache[i-1] + cache[i-3] + cache[i-4];
    }
}

/* Public API to return the nth term of A115008. */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
