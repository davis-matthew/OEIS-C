/*
 * A052759  E.g.f.: x^3*log(1/(1-x)).
 * https://oeis.org/A052759
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A052759
 * Formula: a(n) = n!/(n-3) for n > 3,  a(0)=a(1)=a(2)=a(3)=0
 * The recurrence a(n) = a(n‑1) * n * (n‑4) / (n‑3) is used for efficiency.
 * Cache results in a static array so calls in any order are fast.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n < 4)                /* first four terms are zero */
        return (seq_t)0;

    /* Allocate or grow the cache to hold index n */
    if (n + 1 > cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();   /* allocation failed */
        cache = tmp;

        /* initialise missing entries that were not present before */
        if (cache_size < 4) {
            for (idx_t i = cache_size; i < 4; ++i)
                cache[i] = (seq_t)0;
        }

        /* compute new terms */
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i < 4) {
                cache[i] = (seq_t)0;
            } else if (i == 4) {
                cache[i] = (seq_t)24;           /* 4!/(1) = 24 */
            } else {
                seq_t val = cache[i-1];
                val *= (seq_t)i;                 /* multiply by i  */
                val /= (seq_t)(i-3);             /* exact division  */
                val *= (seq_t)(i-4);             /* multiply by (i‑4) */
                cache[i] = val;
            }
        }

        cache_size = new_size;
    }

    return cache[n];
}
