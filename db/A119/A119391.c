/*
 * A119391  a(n) = n!*Sum_{k=0..n} Stirling1(n,k)/k!.
 * https://oeis.org/A119391
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = n!*Sum_{k=0..n} Stirling1(n,k)/k!
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 9, i64 n <= 14, i128 n <= 2,
 *            big any n
 * Validated: partial, pass, 19 terms through n = 18 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 1, computed for all n >= 0 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

/* the code below runs in wide_t: 128 bits at the fixed widths (never narrower
   than seq_t), seq_t itself for big, so values that pass 2^64 on the way to a
   term that fits stay exact; a() returns the term in seq_t */
#ifdef __cplusplus
typedef seq_t wide_t;
#else
typedef __typeof__((seq_t)0 + (__int128)0) wide_t;
#endif
#define seq_t wide_t

/* seq_t and idx_t are provided by the build environment. */

seq_t a_wide(idx_t n) {
    /* static state for memoization */
    static idx_t computed_max = 0;        /* highest n computed so far   */
    static seq_t *fact = NULL;            /* factorials 0..computed_max   */
    static seq_t *cur  = NULL;            /* Stirling numbers row (s(n,k))*/
    static seq_t *cache= NULL;            /* cache of a_wide(n) values         */
    static size_t  cache_cap = 0;         /* capacity of arrays           */

    /* Allocate/resize arrays if necessary */
    if (n >= cache_cap) {
        size_t new_cap = n + 1;
        /* resize factorial array */
        fact = mem_resize(seq_t, fact, new_cap);
        /* resize Stirling row array */
        cur = mem_resize(seq_t, cur, new_cap);
        /* resize a_wide(n) cache array */
        cache = mem_resize(seq_t, cache, new_cap);
        /* initialize new entries of factorial */
        if (computed_max == 0) {
            fact[0] = 1;
            cur[0]  = 1;      /* s(0,0) = 1  */
            cache[0] = 1;      /* a_wide(0) = 1    */
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

        /* Compute a_wide(m) = sum_{k=0..m} cur[k] * (m! / k!) */
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

#undef seq_t
seq_t a(idx_t n)
{
    return (seq_t)a_wide(n);
}
