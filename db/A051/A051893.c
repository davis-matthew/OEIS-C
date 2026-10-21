/*
 * A051893  a(n) = Sum_{i=1..n-1} i^2*a(i), a(1) = 1.
 * https://oeis.org/A051893
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nice, nonn
 * Formula:   a(n) = Product_{i=2..n-1} (i^2+1), a(1)=1 (empty product =1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses the recurrence a(n) = a(n‑1) * ((n-1)^2 + 1).    */
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static 1‑indexed cache */
    static seq_t *cache   = NULL; /* array of seq_t values   */
    static idx_t  max_idx = 0;    /* largest index that is computed */
    static idx_t  capacity = 0;   /* allocated size (in elements)   */

    if (n < 1)          /* undefined; return 0 for safety */
        return (seq_t)0;

    /* initialise cache on first call */
    if (cache == NULL) {
        capacity = 3;   /* indices 0,1,2 (index 0 unused) */
        cache = (seq_t *)realloc(NULL, capacity * sizeof(seq_t));
        if (!cache)
            return (seq_t)0; /* allocation fail */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        max_idx  = 2;
    }

    /* if requested n is already computed, return it */
    if (n <= max_idx)
        return cache[n];

    /* grow the allocated storage if needed   */
    if (capacity <= n) {
        idx_t new_capacity = n + 1;      /* include index n */
        seq_t *tmp = (seq_t *)realloc(cache, new_capacity * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;            /* allocation fail */
        cache    = tmp;
        capacity = new_capacity;
    }

    /* compute missing values up to n */
    for (idx_t i = max_idx + 1; i <= n; ++i) {
        seq_t im1 = (seq_t)(i - 1);          /* (i-1) as seq_t */
        seq_t factor = im1 * im1 + (seq_t)1; /* (i-1)^2 + 1 */
        cache[i] = cache[i - 1] * factor;
    }
    max_idx = n;
    return cache[n];
}
