/*
 * A030195  a(n) = 3*a(n-1) + 3*a(n-2), a(0)=0, a(1)=1.
 * https://oeis.org/A030195
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 3*a(n-2), a(0) = 0, a(1) = 1
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 34, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Sequence function */
seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t computed_up_to = 1;   /* Highest index for which cache is valid */

    /* Handle the base cases explicitly */
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)1;

    /* Initialise cache once */
    if (cache == NULL) {
        cache_len = 2;  /* indices 0 and 1 */
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
    }

    /* Grow the cache if necessary */
    if (cache_len <= n) {
        idx_t new_len = n + 1;
        /* Optional: grow a bit more to amortise reallocations */
        if (new_len < cache_len * 2)
            new_len = cache_len * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (tmp == NULL)  /* allocation failure – abort */
            return (seq_t)0;
        cache = tmp;
        cache_len = new_len;
    }

    /* Compute terms up to n if they haven't been computed yet */
    if (computed_up_to < n) {
        idx_t start = computed_up_to + 1;
        if (start < 2)
            start = 2;
        for (idx_t i = start; i <= n; ++i) {
            /* a(i) = 3*a(i-1) + 3*a(i-2) */
            cache[i] = ((seq_t)3 * cache[i-1]) + ((seq_t)3 * cache[i-2]);
        }
        computed_up_to = n;
    }

    return cache[n];
}
