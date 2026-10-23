/*
 * A163993  Number of reduced words of length n in Coxeter group on 25
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^6 = I.
 * https://oeis.org/A163993
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 23*(a(n-1)+a(n-2)+a(n-3)+a(n-4)+a(n-5)) * - 276*a(n-6) +
 *            c(n) * where c(0)=1, c(1..5)=2, c(6)=1, c(k)=0 for k>6
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation using a linear recurrence based on the ordinary generating
 * function:
 *   (t^6 + 2*t^5 + 2*t^4 + 2*t^3 + 2*t^2 + 2*t + 1)
 *   ------------------------------------------------------
 *   (276*t^6 - 23*t^5 - 23*t^4 - 23*t^3 - 23*t^2 - 23*t + 1)
 *
 *  The coefficients of the denominator give a recurrence of order 6.
 *  All arithmetic is performed in seq_t, which is supplied by the build.
 *  The function is memoised with a static cache that grows as needed. */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Static cache of previously computed terms.  */
    static seq_t *cache = NULL;
    static size_t  cap   = 0;   /* allocated capacity (in terms)   */
    static size_t  len   = 0;   /* number of terms stored so far   */

    /* Convert N to a size_t for indexing into the array.  */
    size_t nsize = (size_t)n;

    /* Allocate/capacity handling.  */
    if (cache == NULL) {
        cap   = 8;                       /* start reasonably large */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
    }
    if (nsize + 1 > cap) {
        while (cap < nsize + 1) {
            cap *= 2;
        }
        seq_t *new_cache = (seq_t *)malloc(cap * sizeof(seq_t));
        memcpy(new_cache, cache, len * sizeof(seq_t));
        free(cache);
        cache = new_cache;
    }

    /*  recurrence coefficients for the first 7 terms  */
    static const int c[7] = {1, 2, 2, 2, 2, 2, 1};

    /* Compute missing terms up to and including n.  */
    for (size_t i = len; i <= nsize; ++i) {
        seq_t sum = (seq_t)0;

        /* add a[i-1] .. a[i-5] if they exist  */
        if (i >= 1) sum += cache[i-1];
        if (i >= 2) sum += cache[i-2];
        if (i >= 3) sum += cache[i-3];
        if (i >= 4) sum += cache[i-4];
        if (i >= 5) sum += cache[i-5];

        /* multiply by 23  */
        sum = sum * (seq_t)23;

        /* subtract 276*a[i-6] if it exists  */
        if (i >= 6) {
            sum = sum - (cache[i-6] * (seq_t)276);
        }

        /* add the inhomogeneous part c[i] if i<7  */
        if (i < 7) {
            sum = sum + (seq_t)c[i];
        }

        cache[i] = sum;
    }

    len = nsize + 1;
    return cache[nsize];
}
