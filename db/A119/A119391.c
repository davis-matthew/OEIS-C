/*
 * A119391  a(n) = n!*Sum_{k=0..n} Stirling1(n,k)/k!.
 * https://oeis.org/A119391
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = n!*Sum_{k=0..n} Stirling1(n,k)/k!
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 9, i64 n <= 14, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 1, computed for all n >= 0 */

#include <stdlib.h>
#include <stdint.h>

/* seq_t and idx_t are provided by the build environment. */

seq_t a(idx_t n) {
    /* static state for memoization */
    static idx_t computed_max = 0;        /* highest n computed so far   */
    static seq_t *fact = NULL;            /* factorials 0..computed_max   */
    static seq_t *cur  = NULL;            /* Stirling numbers row (s(n,k))*/
    static seq_t *cache= NULL;            /* cache of a(n) values         */
    static size_t  cache_cap = 0;         /* capacity of arrays           */

    /* Allocate/resize arrays if necessary */
    if (n >= cache_cap) {
        size_t new_cap = n + 1;
        /* resize factorial array */
        fact = (seq_t*)realloc(fact, new_cap * sizeof(seq_t));
        /* resize Stirling row array */
        cur = (seq_t*)realloc(cur,  new_cap * sizeof(seq_t));
        /* resize a(n) cache array */
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        /* initialize new entries of factorial */
        if (computed_max == 0) {
            fact[0] = 1;
            cur[0]  = 1;      /* s(0,0) = 1  */
            cache[0] = 1;      /* a(0) = 1    */
        }
        for (idx_t i = computed_max + 1; i < new_cap; ++i)
            fact[i] = 0;
        cache_cap = new_cap;
    }

    /* Compute up to requested n */
    for (idx_t m = computed_max + 1; m <= n; ++m) {
        /* update Stirling row from previous n=m-1 to n=m */
        /* in-place backward update: s(m,k) = s(m-1,k-1) - (m-1)*s(m-1,k) */
        for (idx_t k = m; k >= 1; --k) {
            cur[k] = cur[k-1] - (seq_t)(m-1) * cur[k];
        }
        cur[0] = 0;                  /* s(m,0) = 0 for m>0          */

        /* Compute a(m) = sum_{k=0..m} cur[k] * (m! / k!) */
        seq_t sum = 0;
        /* ratio = (m! / k!) computed iteratively from k=m down to 0 */
        seq_t ratio = 1;             /* when k = m, ratio = 1        */
        for (idx_t k = m; (int64_t)k >= 0; --k) {
            if (k < m)
                ratio = ratio * (seq_t)(k+1);   /* multiply by (k+1)   */
            sum += cur[k] * ratio;
        }
        cache[m] = sum;
    }
    computed_max = n;
    return cache[n];
}
