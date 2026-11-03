/*
 * A206453  Number of 0..6 arrays of length n avoiding the consecutive pattern
 *          0..6.
 * https://oeis.org/A206453
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) - a(n-7) for n>=7, with a(0)=1, a(1)=7, a(2)=49,
 *            a(3)=343, a(4)=2401, a(5)=16807, a(6)=117649
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 202 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t max_computed = (idx_t)-1;   /* no values computed yet */

    size_t required = (size_t)n + 1;
    if (required > cache_cap) {
        seq_t *tmp = (cache == NULL) ? seq_alloc(required)
                                      : seq_realloc(cache, cache_cap, required);
        if (tmp == NULL) abort();   /* allocation failure */
        cache = tmp;
        cache_cap = required;
    }

    idx_t start = (max_computed == (idx_t)-1) ? 0 : max_computed + 1;
    for (idx_t i = start; i <= n; ++i) {
        if (i < 7) {
            if (i == 0) {
                cache[i] = (seq_t)1;
            } else {
                cache[i] = cache[i - 1] * (seq_t)7;
            }
        } else {
            cache[i] = cache[i - 1] * (seq_t)7 - cache[i - 7];
        }
    }
    max_computed = n;
    return cache[n];
}
