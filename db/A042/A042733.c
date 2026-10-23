/*
 * A042733  Denominators of continued fraction convergents to sqrt(896).
 * https://oeis.org/A042733
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 27, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A042733: denominators of continued fraction convergents to sqrt(896).  */
/*  The continued fraction of sqrt(896) is: [29; 1,13,1,58, 1,13,1,58, ...].  */
/*  The denominators q_n satisfy:                    */
/*        q_{-2} = 1,  q_{-1} = 0                         */
/*        q_0   = 29*q_{-1} + q_{-2} = 1                  */
/*        q_n   = a_n*q_{n-1} + q_{n-2}   for n >= 1     */
/*  where a_0 = 29 and for n >= 1, a_n cycles 1,13,1,58.   */

#include <stdlib.h>   /* malloc, realloc */

seq_t a(idx_t n)
{
    /* static cache of previously computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of terms stored (index+1) */
    static size_t cache_cap = 0;   /* allocated capacity */

    /* If the requested index is already cached */
    if (n < cache_len)
        return cache[n];

    /* Prepare the state for generating new terms */
    seq_t prev_prev, prev;
    if (cache_len == 0) {           /* before a_0 */
        prev_prev = (seq_t)1;       /* q_{-2} */
        prev      = (seq_t)0;       /* q_{-1} */
    } else if (cache_len == 1) {    /* only q_0 stored */
        prev_prev = (seq_t)0;       /* q_{-1} */
        prev      = cache[0];       /* q_0   */
    } else {                        /* at least q_{len-2} and q_{len-1} stored */
        prev_prev = cache[cache_len - 2];
        prev      = cache[cache_len - 1];
    }

    /* Grow the cache if necessary */
    if (cache_cap == 0) {
        cache_cap = 16; /* initial capacity */
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
    }
    /* Ensure capacity up to n+1 terms */
    if (cache_len + (n - cache_len + 1) > cache_cap) {
        size_t new_cap = cache_cap > 0 ? cache_cap * 2 : 16;
        while (new_cap < cache_len + (n - cache_len + 1))
            new_cap *= 2;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Generate missing terms up to index n */
    for (idx_t k = cache_len; k <= n; ++k) {
        seq_t a_k;
        if (k == 0) {
            a_k = (seq_t)29;
        } else {
            switch ((k - 1) & 3) {   /* cycle 1,13,1,58 */
                case 0: a_k = (seq_t)1;  break;
                case 1: a_k = (seq_t)13; break;
                case 2: a_k = (seq_t)1;  break;
                default: a_k = (seq_t)58; break;
            }
        }
        seq_t q = a_k * prev + prev_prev;
        cache[cache_len++] = q;
        prev_prev = prev;
        prev = q;
    }

    return cache[n];
}
