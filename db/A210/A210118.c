/*
 * A210118  Number of (n+1) X 2 0..3 arrays with every 2 X 2 subblock having
 *          two or four distinct values, and new values 0..3 introduced in row
 *          major order.
 * https://oeis.org/A210118
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n-1) - 19*a(n-2) - 14*a(n-3) with a(1)=8, a(2)=44,
 *            a(3)=265 (offset = 1)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Return the nth term (a(n)) of OEIS A210118.
   The indices and values are defined by the types idx_t and seq_t,
   supplied by the build environment. */
seq_t a(idx_t n)
{
    /* Cache of previously computed values:
       cache[0] corresponds to a(1) */
    static seq_t *cache = NULL;
    static idx_t  cache_len = 0;   /* number of entries actually allocated */

    /* Sequence is defined only for n ≥ 1 */
    if (n < 1) return (seq_t)0;

    /* First call – allocate at least the three known terms */
    if (cache == NULL) {
        size_t init = (size_t)(n > 3 ? n : 3);      /* space needed now */
        cache = (seq_t *)malloc(init * sizeof(seq_t));
        if (!cache) return (seq_t)0;                /* out of memory */

        /* Initial known values */
        cache[0] = (seq_t)8;        /* a(1) */
        cache[1] = (seq_t)44;       /* a(2) */
        cache[2] = (seq_t)265;      /* a(3) */
        cache_len = (idx_t)init;

        /* Compute additional terms up to n (if needed) */
        for (idx_t i = 3; i < n; ++i) {
            cache[i] =
                (seq_t)10 * cache[i-1]
                - (seq_t)19 * cache[i-2]
                - (seq_t)14 * cache[i-3];
        }
    }
    /* Subsequent calls – grow the cache if required */
    else if (cache_len < n) {
        size_t new_size = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;               /* allocation failure */
        cache = tmp;

        /* Compute the missing terms */
        for (idx_t i = cache_len; i < n; ++i) {
            cache[i] =
                (seq_t)10 * cache[i-1]
                - (seq_t)19 * cache[i-2]
                - (seq_t)14 * cache[i-3];
        }
        cache_len = (idx_t)new_size;
    }

    return cache[n-1];   /* a(n) is stored at index n-1 */
}
