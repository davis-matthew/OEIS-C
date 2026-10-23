/*
 * A195971  Number of n X 1 0..4 arrays with each element x equal to the
 *          number its horizontal and vertical neighbors equal to 2,0,1,3,4
 *          for x=0,1,2,3,4.
 * https://oeis.org/A195971
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-3) + a(n-4), with a(0)=0, a(1)=1, a(2)=3,
 *            a(3)=4;
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 46, u64 n <= 92, u128 n <= 184,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A195971. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static const seq_t base_vals[4] = {(seq_t)0, (seq_t)1, (seq_t)3, (seq_t)4};

    /* Handle small indices directly to avoid any allocation */
    if (n < 4) {
        return base_vals[n];
    }

    /* First call: allocate initial cache for first 4 values */
    if (cache == NULL) {
        cache_size = 4;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return (seq_t)0;  /* allocation failed */
        cache[0] = base_vals[0];
        cache[1] = base_vals[1];
        cache[2] = base_vals[2];
        cache[3] = base_vals[3];
    }

    /* If the requested index is beyond current cache, grow the cache */
    if (n >= cache_size) {
        idx_t old_size = cache_size;
        idx_t new_size = n + (idx_t)1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;  /* allocation failed */
        cache = tmp;
        /* Initialise new entries to zero (not strictly necessary, but clear) */
        for (idx_t i = old_size; i < new_size; ++i) {
            cache[i] = (seq_t)0;
        }
        cache_size = new_size;

        /* Compute values from old_size up to n */
        for (idx_t i = old_size; i <= n; ++i) {
            cache[i] = cache[i-1] + cache[i-3] + cache[i-4];
        }
    }

    /* If n < cache_size, the value has already been computed */
    return cache[n];
}
