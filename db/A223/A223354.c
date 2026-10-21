/*
 * A223354  Rolling cube footprints: number of n X 5 0..5 arrays starting with
 *          0 where 0..5 label faces of a cube and every array movement to a
 *          horizontal or antidiagonal neighbor moves across a corresponding
 *          cube edge.
 * https://oeis.org/A223354
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 3, u64 n <= 7, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=256, a(2)=110592,
   a(n)=480*a(n-1)-18432*a(n-2)  for n>=3 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for the sequence, return 0 */
    if (n == 1) return (seq_t)256;
    if (n == 2) return (seq_t)110592;

    /* Static cache for previously computed values.
       cache[i] holds a(i+1).  cache_len == number of cached values. */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* Initialise cache with first two terms */
    if (cache == NULL) {
        cache_len = 2;
        cache = (seq_t*)malloc(cache_len * sizeof(seq_t));
        cache[0] = (seq_t)256;     /* a(1) */
        cache[1] = (seq_t)110592;  /* a(2) */
    }

    /* Return cached value if available */
    if (n <= cache_len)
        return cache[n-1];

    /* Extend cache up to n, computing new terms one by one */
    idx_t i;
    idx_t old_len = cache_len;
    cache_len = n;
    cache = (seq_t*)realloc(cache, cache_len * sizeof(seq_t));
    const seq_t C1 = (seq_t)480;
    const seq_t C2 = (seq_t)18432;

    for (i = old_len + 1; i <= n; ++i) {
        cache[i-1] = C1 * cache[i-2] - C2 * cache[i-3];
    }

    return cache[n-1];
}
