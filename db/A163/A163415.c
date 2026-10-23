/*
 * A163415  a(n) = 18*a(n-1) - 79*a(n-2) for n>1, a(0)=1, a(1)=13.
 * https://oeis.org/A163415
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 18*a(n-1) - 79*a(n-2) for n>1, a(0)=1, a(1)=13
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#ifndef __cplusplus
#include <stdint.h>
#endif

static seq_t *seq_cache = NULL;
static size_t seq_cap   = 0;
static idx_t  seq_max   = (idx_t)-1;

/* Grow the cache to at least `needed` elements. */
static void seq_ensure_cap(size_t needed)
{
    if (seq_cap >= needed)
        return;
    size_t new_cap = seq_cap ? seq_cap : 2;
    while (new_cap < needed)
        new_cap <<= 1;
#ifdef __cplusplus
    seq_t *tmp = new seq_t[new_cap];
    for (size_t i = 0; i < seq_cap; ++i)
        tmp[i] = seq_cache[i];
    delete [] seq_cache;
    seq_cache = tmp;
#else
    seq_t *tmp = realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();
    seq_cache = tmp;
#endif
    seq_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* First call: initialise cache with base cases. */
    if (!seq_cache) {
        seq_ensure_cap(2);
#ifndef __cplusplus
        /* zeroing helps trivial types */
        for (size_t i = 0; i < seq_cap; ++i)
            seq_cache[i] = (seq_t)0;
#endif
        seq_cache[0] = (seq_t)1;
        seq_cache[1] = (seq_t)13;
        seq_max = 1;
    }

    /* Ensure capacity for requested index. */
    seq_ensure_cap((size_t)n + 1);

    /* Compute any missing terms up to n. */
    if (n > seq_max) {
        for (idx_t i = seq_max + 1; i <= n; ++i) {
            seq_cache[i] = (seq_t)18 * seq_cache[i - 1] -
                           (seq_t)79 * seq_cache[i - 2];
        }
        seq_max = n;
    }

    return seq_cache[n];
}
