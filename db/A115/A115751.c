/*
 * A115751  a(1)=1. a(n) = number of positive divisors of n which are not
 *          among the first (n-1) terms of the sequence.
 * https://oeis.org/A115751
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9738, u16 n <= 9738, u32 n <= 9738, u64 n <= 9738,
 *            u128 n <= 9738, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A115751: a(1)=1. 
   a(n) = number of positive divisors of n that are not among
   a(1),a(2),…,a(n‑1). */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* seq_t and idx_t are supplied by the build system; no typedefs here. */

seq_t a(idx_t n)
{
    /* Static cache of computed terms and a bitmap that records
       which values have already appeared in the sequence. */
    static seq_t *cache   = NULL;   /* cache[1..maxComputed] */
    static bool  *seen    = NULL;   /* seen[value] == true if value seen */
    static idx_t  maxComputed = 0;   /* largest n for which cache is valid */

    /* Initialization on first call. */
    if (cache == NULL) {
        maxComputed = 1;
        cache = (seq_t *)malloc(2 * sizeof(seq_t));   /* indices 0..1 */
        seen  = (bool *)calloc(2, sizeof(bool));      /* indices 0..1 */

        cache[1] = (seq_t)1;
        seen[1] = true;
    }

    /* Extend the cache if necessary.  Calls are allowed in any order. */
    if (n > maxComputed) {
        idx_t newSize = n + 1;  /* array indices 0..n */

        cache = (seq_t *)realloc(cache, (size_t)newSize * sizeof(seq_t));
        seen  = (bool  *)realloc(seen,  (size_t)newSize * sizeof(bool));

        /* New entries of seen are initially false. */
        memset(seen + maxComputed + 1,
               0, (size_t)(newSize - maxComputed - 1) * sizeof(bool));

        for (idx_t i = maxComputed + 1; i <= n; ++i) {
            seq_t cnt = 0;

            /* Count divisors of i that have not appeared yet. */
            for (idx_t d = 1; d <= i / d; ++d) {
                if (i % d == 0) {
                    if (!seen[(size_t)d]) ++cnt;
                    idx_t d2 = i / d;
                    if (d2 != d && !seen[(size_t)d2]) ++cnt;
                }
            }

            cache[i] = cnt;
            /* Mark that this value now appears in the sequence. */
            seen[(size_t)cnt] = true;
        }

        maxComputed = n;
    }

    return cache[n];
}
