/*
 * A178441  INVERT transform of A058187, the tetrahedral numbers with repeats.
 * https://oeis.org/A178441
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=2, a(2)=7, a(3)=17, a(4)=46, a(5)=119, a(6)=311,
 *            a(n)=2*a(n-1)+3*a(n-2)-3*a(n-3)-3*a(n-4)+3*a(n-5)+a(n-6)-a(n-7)
 *            for n≥7 (generating function
 *            1/(1-2x-3x^2+3x^3+3x^4-3x^5-x^6+x^7)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Static cache to keep already computed values for arbitrary call order. */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;   /* number of elements currently stored (next free index) */
    static size_t cache_cap  = 0;   /* size of allocated array */

    /* Ensure the cache is large enough to hold the requested index. */
    if (cache_len == 0) {
        /* First call: allocate minimum capacity to hold base cases. */
        cache_cap = 7 + 1;      /* indices 0..6 */
        cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* allocation failed */

        /* initialise base values */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)7;
        cache[3] = (seq_t)17;
        cache[4] = (seq_t)46;
        cache[5] = (seq_t)119;
        cache[6] = (seq_t)311;
        cache_len = 7;          /* next index to compute */
    }

    if ((idx_t)n >= (idx_t)cache_cap) {
        /* Grow the cache to fit n.  Use realloc to preserve existing data. */
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing values up to n. */
    for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
        if (i <= 6) {
            /* already initialized during first allocation */
            continue;
        }
        /* recurrence for i >= 7 */
        cache[i] =
            (seq_t)2 * cache[i - 1] +
            (seq_t)3 * cache[i - 2] -
            (seq_t)3 * cache[i - 3] -
            (seq_t)3 * cache[i - 4] +
            (seq_t)3 * cache[i - 5] +
            cache[i - 6] -
            cache[i - 7];
    }

    cache_len = (size_t)n + 1;   /* all indices up to n are now stored */
    return cache[n];
}
