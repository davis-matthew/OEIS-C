/*
 * A076969  a(1) = 1, a(n+1)= smallest cube greater than the n-th partial sum.
 * https://oeis.org/A076969
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1, a(n+1) = smallest cube greater than the n-th partial
 *            sum
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 27, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Validated: partial, pass, 29 terms through n = 29 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"

/* Floor cube root by binary search */
static inline seq_t floor_cube_root(seq_t x)
{
    seq_t lo = 0, hi = 1;
    /* raise hi until hi^3 > x */
    while (ipow_i32(hi, 3) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid3 = ipow_i32(mid, 3);
        if (mid3 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
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
    /* static cache */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static idx_t  cache_len  = 0;
    static seq_t sum         = 0;

    /* produce terms up to n */
    while (cache_len < n)
    {
        /* grow the cache if needed */
        if (cache_len >= cache_cap)
        {
            size_t new_cap = cache_cap ? cache_cap * 2 : 4;
            seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
            cache = tmp;
            cache_cap = new_cap;
        }

        /* next term: smallest cube > sum */
        seq_t k = floor_cube_root(sum);
        k += (seq_t)1;                       /* cube root of the next cube */
        seq_t cube = k * k * k;              /* the cube itself */

        cache[cache_len] = cube;              /* store */
        sum += cube;                         /* update partial sum */
        ++cache_len;
    }

    return cache[n - 1];
}
