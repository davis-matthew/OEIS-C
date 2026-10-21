/*
 * A051736  Number of 3 X n (0,1)-matrices with no consecutive 1's in any row
 *          or column.
 * https://oeis.org/A051736
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   a(n) = 2*a(n-1) + 6*a(n-2) - a(n-4) for n ≥ 4, * with initial
 *            values a(0)=1, a(1)=5, a(2)=17, a(3)=63
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 34, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* seq_t and idx_t are defined by the build system. */
seq_t a(idx_t n)
{
    /* Static cache to store already computed terms. */
    static seq_t *cache   = NULL;   /* pointer to array of seq_t values */
    static idx_t  cache_len = 0;    /* number of valid elements in cache */

    /* Initialise cache with first four terms if needed. */
    if (cache_len == 0) {
        cache_len = 4;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* out‑of‑memory: return 0 */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)5;
        cache[2] = (seq_t)17;
        cache[3] = (seq_t)63;
    }

    /* Extend cache up to index n if required. */
    if (n >= cache_len) {
        /* Resize array. */
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* out‑of‑memory: return 0 */
        cache = tmp;

        /* Compute new terms. */
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i < 4) {
                /* The first four values are already set in the initial block. */
                continue;
            }
            /* Recurrence: a(i) = 2*a(i-1) + 6*a(i-2) - a(i-4). */
            cache[i] = (seq_t)2 * cache[i - 1]
                     + (seq_t)6 * cache[i - 2]
                     -          cache[i - 4];
        }
        cache_len = n + 1;
    }

    return cache[n];
}
