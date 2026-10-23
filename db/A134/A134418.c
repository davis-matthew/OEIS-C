/*
 * A134418  Row sums of triangle A134417.
 * https://oeis.org/A134418
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 4*a(n-2), n >= 2 a(0) = 1, a(1) = 5 G.f.:
 *            (1+3*x)/(1-2*x-4*x^2)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 37, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, free */

/* Static cache for previously computed terms */
static seq_t *cache = NULL;   /* pointers to elements a[0] .. a[cache_len-1] */
static idx_t cache_len = 0;  /* number of terms currently stored */

/* Return the n-th term of OEIS A134418 with memoisation. */
seq_t a(idx_t n)
{
    /* Handle the two base cases explicitly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;

    /* Ensure the cache holds at least indices up to n */
    if (cache_len <= n) {
        /* Grow the dynamic array to size n+1 */
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failure – abort.  This path is unlikely in
               contest settings but keeps the function safe. */
            exit(EXIT_FAILURE);
        }
        cache = tmp;

        /* Initialise the first two entries if necessary */
        if (cache_len == 0) {
            cache[0] = (seq_t)1;
            if (new_len > 1) cache[1] = (seq_t)5;
        } else if (cache_len == 1) {
            cache[1] = (seq_t)5;
        }

        /* Compute all missing terms using the recurrence */
        idx_t start = (cache_len > 1 ? cache_len : (idx_t)2);
        for (idx_t i = start; i <= n; ++i) {
            cache[i] = 2 * cache[i - 1] + 4 * cache[i - 2];
        }

        cache_len = new_len;
    }

    return cache[n];
}
