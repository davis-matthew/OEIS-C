/*
 * A145448  a(n) = 12^n*n!.
 * https://oeis.org/A145448
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12^n * n!
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 10, u128 n <= 19,
 *            big any n
 * Validated: standard, pass, 30 terms through n = 299 (2026-11-01)
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

seq_t a(idx_t n)
{
    /* static cache: a(0) = 1, a(k) = a(k-1) * 12 * k   for k >= 1 */
    static seq_t *cache = NULL;   /* array of pre‑computed terms */
    static size_t size     = 0;   /* number of terms actually stored (cache[0..size-1]) */

    /* grow the cache if necessary */
    if (size <= n) {
        size_t new_cap = size? size << 1 : 1;
        while (new_cap <= n) new_cap <<= 1;
        cache = seq_realloc(cache, size, new_cap);
        /* initialize new slots (unnecessary for big integers, but silence compiler warnings) */
    }

    /* compute missing terms */
    for (idx_t i = size; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;
        } else {
            seq_t mult = (seq_t)12 * (seq_t)i;   /* 12 * i as seq_t */
            cache[i] = cache[i-1] * mult;
        }
    }
    size = n + 1;   /* we now have values up to a(n) */

    return cache[n];
}
