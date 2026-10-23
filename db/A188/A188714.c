/*
 * A188714  G.f.: (1+x+x^2+x^3)/(1-3*x-3*x^2-3*x^3).
 * https://oeis.org/A188714
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=4, a(2)=16, a(3)=64,
   a(n)=3*(a(n-1)+a(n-2)+a(n-3)) for n>=4. */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* array of already computed terms */
    static size_t cache_cap = 0;  /* allocated capacity of cache */
    static size_t cache_size = 0; /* number of terms currently stored (valid indices 0..cache_size-1) */

    size_t idx = (size_t)n;          /* convert index for array access */

    /* Lazily allocate and initialise the cache on the first call. */
    if (!cache) {
        cache_cap  = 16;             /* initial allocation size */
        cache      = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) return (seq_t)0; /* allocation failure: return 0 */

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)16;
        cache[3] = (seq_t)64;
        cache_size = 4;              /* indices 0..3 are valid */
    }

    /* Grow the cache if the requested index is beyond the current capacity. */
    if (idx >= cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= idx) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp) return (seq_t)0;   /* failed to grow; return 0 */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to idx, keeping cache_size up to date. */
    if (idx >= cache_size) {
        for (size_t i = cache_size; i <= idx; ++i) {
            /* For i < 3 the recurrence is not used; this block only reaches i >= 4. */
            seq_t sum = cache[i - 1] + cache[i - 2] + cache[i - 3];
            cache[i] = 3 * sum;
        }
        cache_size = idx + 1;
    }

    return cache[idx];
}
