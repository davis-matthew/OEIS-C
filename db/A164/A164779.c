/*
 * A164779  Number of reduced words of length n in Coxeter group on 10
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^8 = I.
 * https://oeis.org/A164779
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is defined by the linear recurrence a(0)=1, a(1)=10,
 *            a(k)=9·a(k-1) –44·a(k-8)+36·a(k-9) for k≥2 (with the terms
 *            a(k-8) or a(k-9) taken as 0 for k<8,9). This recurrence follows
 *            from the generating function (1+x)(1-x^8)/(1-9*x+44*x^8-36*x^9)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Dynamic cache of already computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of allocated slots */

    /* Determine the target length of the cache */
    size_t need = (size_t)n + 1;
    if (need < (size_t)10) need = 10;   /* at least capacity for first 10 terms */

    /* Allocate or grow the cache if necessary */
    if (cache == NULL) {
        /* first allocation */
        cache = (seq_t*)calloc(need, sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;   /* allocation failure */
        cache_len = need;

        /* Compute the first `need` terms */
        for (size_t i = 0; i < cache_len; ++i) {
            if (i == 0) {
                cache[i] = (seq_t)1;                            /* a(0) */
            } else if (i == 1) {
                cache[i] = (seq_t)9 * cache[i-1] + (seq_t)1;    /* a(1) */
            } else if (i < 8) {
                /* a(i) = 9·a(i-1) for 2 ≤ i ≤ 7 */
                cache[i] = (seq_t)9 * cache[i-1];
            } else if (i == 8) {
                /* a(8) = 9·a(7) –44·a(0) –1 */
                cache[i] = (seq_t)9 * cache[i-1]
                         - (seq_t)44 * cache[0]
                         - (seq_t)1;
            } else if (i == 9) {
                /* a(9) = 9·a(8) –44·a(1) +36·a(0) –1 */
                cache[i] = (seq_t)9 * cache[i-1]
                         - (seq_t)44 * cache[1]
                         + (seq_t)36 * cache[0]
                         - (seq_t)1;
            } else {
                /* a(i) = 9·a(i-1) –44·a(i-8) +36·a(i-9) for i ≥ 10 */
                cache[i] = (seq_t)9 * cache[i-1]
                         - (seq_t)44 * cache[i-8]
                         + (seq_t)36 * cache[i-9];
            }
        }
        return cache[n];
    }

    /* If the requested index is already cached, return it */
    if ((size_t)n < cache_len) {
        return cache[n];
    }

    /* Grow the cache to hold up to index n */
    size_t new_len = need;
    seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
    if (tmp == NULL) return (seq_t)0;   /* allocation failure */
    cache = tmp;

    /* Compute new terms from old_len up to n */
    for (size_t i = cache_len; i < new_len; ++i) {
        if (i == 0) {
            cache[i] = (seq_t)1;
        } else if (i == 1) {
            cache[i] = (seq_t)9 * cache[i-1] + (seq_t)1;
        } else if (i < 8) {
            cache[i] = (seq_t)9 * cache[i-1];
        } else if (i == 8) {
            cache[i] = (seq_t)9 * cache[i-1]
                     - (seq_t)44 * cache[0]
                     - (seq_t)1;
        } else if (i == 9) {
            cache[i] = (seq_t)9 * cache[i-1]
                     - (seq_t)44 * cache[1]
                     + (seq_t)36 * cache[0]
                     - (seq_t)1;
        } else {
            cache[i] = (seq_t)9 * cache[i-1]
                     - (seq_t)44 * cache[i-8]
                     + (seq_t)36 * cache[i-9];
        }
    }
    cache_len = new_len;
    return cache[n];
}
