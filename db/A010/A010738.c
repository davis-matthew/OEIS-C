/*
 * A010738  Shifts 2 places right under binomial transform.
 * https://oeis.org/A010738
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, eigen
 * Formula:   a(0) = 1, a(1) = 1 for n >= 2: a(n) = a(n-2) - Σ_{k=0}^{n-1}
 *            binom(n,k) * a(k) (i.e., the binomial transform of {a(n)} shifts
 *            the sequence two places to the right)
 * Range:     i8 n <= 6, i16 n <= 10, i32 n <= 17, i64 n <= 23, i128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;     /* previously computed terms          */
    static size_t cache_cap = 0;    /* allocated capacity                  */
    static size_t cache_len = 0;    /* number of terms stored (max index+1) */

    /* Allocate cache on first call */
    if (cache == NULL) {
        cache_cap = (n >= 3 ? n + 1 : 8);
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_len = 0;
    }

    /* Initialise first two terms once */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache_len = 2;
    }

    /* Extend cache if n exceeds current stored indices */
    if (n >= cache_len) {
        if (n >= cache_cap) {               /* grow capacity if needed */
            size_t new_cap = cache_cap;
            while (n >= new_cap) new_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Compute missing terms up to n */
        for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
            seq_t sum  = 0;
            seq_t binom = 1;                 /* binom(i,0) */

            for (idx_t k = 0; k < i; ++k) { /* k ranges 0 .. i-1            */
                if (k > 0) {
                    seq_t numer = binom * (seq_t)(i - (k - 1));
                    binom = numer / (seq_t)k; /* exact division          */
                }
                sum += binom * cache[k];
            }

            seq_t prev = cache[i - 2];       /* a(i-2) is already known      */
            cache[i]  = prev - sum;         /* recurrence                 */
        }

        cache_len = (size_t)n + 1;         /* highest index + 1          */
    }

    return cache[n];
}
