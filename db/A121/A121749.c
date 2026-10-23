/*
 * A121749  Number of deco polyominoes of height n, consisting only of columns
 *          of odd length. A deco polyomino is a directed column-convex
 *          polyomino in which the height, measured along the diagonal, is
 *          attained only in the last column.
 * https://oeis.org/A121749
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 15, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Sequence A121749: Number of deco polyominoes of height n with all columns odd.
   Recurrence: a(n)=floor(n/2)*(a(n-1)+a(n-2)),  a(1)=a(2)=1 */
seq_t a(idx_t n)
{
    /* Handle out‑of‑range indices (behaviour below offset doesn't matter) */
    if (n < 1) return (seq_t)0;

    /* Base values */
    if (n == 1 || n == 2) return (seq_t)1;

    /* Static cache for memoisation */
    static seq_t *cache = NULL;          /* array 1..max_cached */
    static idx_t max_cached = 0;        /* largest n cached so far */

    /* Initialise cache dynamically on first call */
    if (cache == NULL) {
        /* Allocate space for indices 0..n (0 unused) */
        cache = (seq_t*)malloc((n + 1) * sizeof(seq_t));
        if (cache == NULL) abort();     /* allocation failure */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        max_cached = 2;
    }

    /* Extend cache if necessary */
    if (n > max_cached) {
        seq_t *new_cache = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));
        if (new_cache == NULL) abort();
        cache = new_cache;
        /* Compute new entries */
        for (idx_t k = max_cached + 1; k <= n; ++k) {
            seq_t half = (seq_t)(k / 2);  /* floor(k/2) */
            cache[k] = half * (cache[k - 1] + cache[k - 2]);
        }
        max_cached = n;
    }

    return cache[n];
}
