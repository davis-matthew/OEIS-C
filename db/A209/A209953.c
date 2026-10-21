/*
 * A209953  Number of (n+1) X 2 0..2 arrays with every 2 X 2 subblock having
 *          one, three or four distinct clockwise edge differences.
 * https://oeis.org/A209953
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A209953
 * Recurrence: a(n) = 6*a(n-1) + 15*a(n-2) + 6*a(n-3)
 * Initial values: a(1)=71, a(2)=567, a(3)=4521
 * The implementation memoises results so that repeated calls are O(1)
 * after the first evaluation to the required index.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for n<1; safe default */

    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static idx_t last_computed = 0;

    /* Grow cache if needed */
    if ((size_t)n >= cache_size) {
        size_t new_size = (size_t)n + 1;   /* 1‑based indexing */
#if defined(__cplusplus)
        if (!cache) {
            cache = new seq_t[new_size];
        } else {
            seq_t *tmp = new seq_t[new_size];
            for (size_t i = 0; i < cache_size; ++i) tmp[i] = cache[i];
            delete [] cache;
            cache = tmp;
        }
#else
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failure */
        cache = tmp;
#endif
        cache_size = new_size;
    }

    /* Compute any missing terms */
    if (n > last_computed) {
        for (size_t i = (size_t)last_computed + 1; i <= (size_t)n; ++i) {
            if (i == 1U) {
                cache[1] = 71;
            } else if (i == 2U) {
                cache[2] = 567;
            } else if (i == 3U) {
                cache[3] = 4521;
            } else {
                cache[i] = cache[i-1] * 6 + cache[i-2] * 15 + cache[i-3] * 6;
            }
        }
        last_computed = n;
    }

    return cache[(size_t)n];
}
