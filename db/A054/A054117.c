/*
 * A054117  T(n,n-2), array T as in A054115.
 * https://oeis.org/A054117
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1)*a(n-1) - n*a(n-2) for n ≥ 5 Initial values: a(2)=1,
 *            a(3)=6, a(4)=30
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for realloc */

seq_t a(idx_t n)
{
    /* handle the trivial base cases directly */
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)6;
    if (n == 4) return (seq_t)30;

    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t capacity = 0;   /* allocated array size */
    static idx_t highest  = 0;   /* largest index computed and stored */

    /* initialise the cache on the first call */
    if (cache == NULL) {
        capacity = n + 1;                 /* sufficient for requested index */
        cache    = (seq_t *)realloc(cache, capacity * sizeof(seq_t));
        if (cache == NULL) abort();       /* allocation failure */
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)6;
        cache[4] = (seq_t)30;
        highest  = 4;
    }
    /* if the requested index is beyond current capacity, enlarge the array */
    if (capacity <= n) {
        capacity = n + 1;
        cache = (seq_t *)realloc(cache, capacity * sizeof(seq_t));
        if (cache == NULL) abort();  /* allocation failure */
    }
    /* if already computed, return the cached value */
    if (highest >= n) return cache[n];

    /* compute new values using the recurrence a(i) = (i+1)*a(i-1) - i*a(i-2) */
    for (idx_t i = highest + 1; i <= n; ++i) {
        seq_t term1 = (seq_t)(i + 1) * cache[i - 1];
        seq_t term2 = (seq_t)i * cache[i - 2];
        cache[i] = term1 - term2;
    }
    highest = n;
    return cache[n];
}
