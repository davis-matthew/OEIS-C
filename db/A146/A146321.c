/*
 * A146321  Inverse binomial transform of A070366.
 * https://oeis.org/A146321
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 8, i16 n <= 16, i32 n <= 32, i64 n <= 33, i128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A146321
   Inverse binomial transform of A070366.
   Generating function: 1 + x*(4+10x+7x^2)/((2x+1)*(1+x+x^2)).
   Recurrence:  a(0)=1, a(1)=4, a(2)=-2, a(3)=1;
                a(n) = -3*a(n‑1) - 3*a(n‑2) - 2*a(n‑3)   for n ≥ 4 */

#include <stdlib.h>

/* memoised cache for computed values */
static seq_t *cache = NULL;
static idx_t  cache_len = 0;

/* Return a(n) with memoisation */
seq_t a(idx_t n)
{
    /* initialise cache with first four terms */
    if (cache == NULL) {
        cache_len = 4;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) abort();          /* allocation failure */
        cache[0] = (seq_t) 1;
        cache[1] = (seq_t) 4;
        cache[2] = (seq_t)-2;
        cache[3] = (seq_t) 1;
    }

    /* if already computed, return it */
    if (n < cache_len)
        return cache[n];

    /* extend cache to size n+1 */
    size_t new_len = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) abort();                /* allocation failure */
    cache = tmp;
    idx_t old_len = cache_len;
    cache_len = (idx_t)new_len;

    /* compute missing terms via recurrence */
    for (idx_t i = (old_len > 4 ? old_len : 4); i <= n; ++i) {
        cache[i] = -(seq_t)3 * cache[i - 1]
                 - (seq_t)3 * cache[i - 2]
                 - (seq_t)2 * cache[i - 3];
    }

    return cache[n];
}
