/*
 * A232732  a(n) = Sum_{k=0..2*n} (-1)^k * binomial(12*n,6*k).
 * https://oeis.org/A232732
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 none, i16 n <= 1, i32 n <= 2, i64 n <= 5, i128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = -2766*a(n-1)-172929*a(n-2)-64*a(n-3) ;
   Initial values: a(1) = -922, a(2) = 2434966, a(3) = -6575675482. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* undefined for n = 0; return 0 for safety */
    if (n == 0) return (seq_t)0;

    /* Cached values for a(1)..a(k) (1‑based index). */
    static seq_t *cache = NULL;      /* pointer to array of seq_t */
    static size_t cache_cap = 0;     /* allocated capacity */
    static size_t cache_len = 0;     /* number of terms currently cached */

    /* If the requested index exceeds the current cache, extend it. */
    if ((size_t)n > cache_len) {
        size_t needed = (size_t)n;

        /* Reallocate memory if needed. */
        if (needed > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 8;
            if (new_cap < needed) new_cap = needed;
            /* Realloc casts to the appropriate pointer type. */
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();  /* memory allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Compute all missing terms, starting from the first new index. */
        for (size_t i = cache_len; i < needed; ++i) {
            idx_t idx = (idx_t) i + 1;   /* 1‑based sequence index */

            if (idx == 1) {
                cache[i] = (seq_t)-922;
            } else if (idx == 2) {
                cache[i] = (seq_t)2434966;
            } else if (idx == 3) {
                cache[i] = (seq_t)-6575675482;
            } else {
                /* Use the recurrence relation. */
                seq_t a1 = cache[idx - 2]; /* a(idx-1)  */
                seq_t a2 = cache[idx - 3]; /* a(idx-2)  */
                seq_t a3 = cache[idx - 4]; /* a(idx-3)  */
                cache[i] = ((seq_t)-2766) * a1
                           + ((seq_t)-172929) * a2
                           + ((seq_t)-64) * a3;
            }
        }
        cache_len = needed;
    }

    return cache[n - 1];  /* idx n corresponds to cache[n-1] */
}
