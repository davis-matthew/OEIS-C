/*
 * A236535  a(n)*Pi is the total length of irregular spiral (center points: 2,
 *          3, 1; pattern 1) after n rotations.
 * https://oeis.org/A236535
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)+a(n-3)-a(n-4) Initial values: a(1)=2, a(2)=5,
 *            a(3)=8, a(4)=10
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* array of computed terms (1‑based) */
static idx_t cache_len = 0;      /* number of elements stored in cache */

/* Returns the n-th term of A236535,  n ≥ 1.  The function is
   self‑contained, memoises intermediate values and works with any
   integer type specified by the build. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* safety, although n ≥ 1 */

    /* Initialise the cache with the first four terms if it is empty. */
    if (cache_len == 0) {
        idx_t init_len = (n > (idx_t)4) ? n : (idx_t)4;
        cache = (seq_t *)malloc(init_len * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)5;
        cache[2] = (seq_t)8;
        cache[3] = (seq_t)10;
        cache_len = init_len;
    }

    /* Grow the cache if the requested term is not yet stored. */
    if (cache_len < n) {
        idx_t new_len = n;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        /* Compute missing terms starting from the first missing index. */
        for (idx_t i = cache_len + 1; i <= new_len; ++i) {
            /* i ≥ 5 in all cases where the loop runs, because cache_len
               is at most 4 at the first iteration. */
            cache[i-1] = cache[i-2] + cache[i-4] - cache[i-5];
        }
        cache_len = new_len;
    }

    return cache[n-1];
}
