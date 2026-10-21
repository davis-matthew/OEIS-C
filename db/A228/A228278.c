/*
 * A228278  Number of n X 3 binary arrays with top left value 1 and no two
 *          ones adjacent horizontally, vertically or nw-se diagonally.
 * https://oeis.org/A228278
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 5*a(n-2) + 4*a(n-3) - a(n-5), with a(0)=0,
 *            a(1)=2, a(2)=3, a(3)=13, a(4)=35, a(5)=112
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache      = NULL;
    static idx_t cache_size  = 0;
    static idx_t computed_to = 0;

    /* The recurrence uses a(0), which is defined as 0 */
    if (n == (idx_t)0) return (seq_t)0;

    /* Ensure the cache can hold indices up to n */
    if (cache_size < n + (idx_t)1) {
        size_t new_size = (size_t)(n + (idx_t)1);
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();          /* allocation failure – safe exit */
        cache      = tmp;
        cache_size = new_size;
    }

    /* Fill any missing terms in the cache */
    for (idx_t i = computed_to + (idx_t)1; i <= n; ++i) {
        if (i == (idx_t)1) {
            cache[i] = (seq_t)2;
        } else if (i == (idx_t)2) {
            cache[i] = (seq_t)3;
        } else if (i == (idx_t)3) {
            cache[i] = (seq_t)13;
        } else if (i == (idx_t)4) {
            cache[i] = (seq_t)35;
        } else if (i == (idx_t)5) {
            cache[i] = (seq_t)112;
        } else {
            /* Recurrence for n ≥ 6. Use unsigned arithmetic; the
               result is always non‑negative for the correct terms. */
            cache[i] = cache[i-1] +
                       ((seq_t)5 * cache[i-2]) +
                       ((seq_t)4 * cache[i-3]) -
                       cache[i-5];
        }
    }

    computed_to = n;
    return cache[n];
}
