/*
 * A009102  Expansion of e.g.f. cos(x)/(1+x).
 * https://oeis.org/A009102
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -n*a(n-1) + (n even ? (-1)^(n/2) : 0)
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 12, i64 n <= 20, i128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* with a(0) = 1  */
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cached_to = 0;      /* last index computed (inclusive) */

    /* if memory not yet allocated, initialise cache up to n */
    if (cache == NULL) {
        cache = (seq_t *)calloc(n + 1, sizeof(seq_t));
        if (cache == NULL) {
            /* allocation failure – fall back to simple computation without cache */
            idx_t i;
            seq_t prev = 1;          /* a(0) */
            if (n == 0) return prev;
            for (i = 1; i <= n; ++i) {
                seq_t term_extra;
                if ((i & 1) == 0) {          /* i even */
                    seq_t half = (i >> 1);
                    term_extra = (half & 1) ? (seq_t)(-1) : (seq_t)1;
                } else {
                    term_extra = (seq_t)0;
                }
                seq_t cur = - (seq_t)i * prev + term_extra;
                prev = cur;
            }
            return prev;
        }
        cached_to = 0;
        cache[0] = (seq_t)1;                /* a(0) = 1 */
    }
    /* extend cache if necessary */
    if (n > cached_to) {
        idx_t new_cap = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (new_cache == NULL) {
            /* allocation failure – compute on the fly */
            idx_t i;
            seq_t prev = cache[cached_to];
            for (i = cached_to + 1; i <= n; ++i) {
                seq_t term_extra;
                if ((i & 1) == 0) {
                    seq_t half = (i >> 1);
                    term_extra = (half & 1) ? (seq_t)(-1) : (seq_t)1;
                } else {
                    term_extra = (seq_t)0;
                }
                seq_t cur = - (seq_t)i * prev + term_extra;
                prev = cur;
            }
            return prev;
        }
        cache = new_cache;
        /* compute missing values */
        for (idx_t i = cached_to + 1; i <= n; ++i) {
            seq_t term_extra;
            if ((i & 1) == 0) {                 /* i even */
                seq_t half = (i >> 1);
                term_extra = (half & 1) ? (seq_t)(-1) : (seq_t)1;
            } else {
                term_extra = (seq_t)0;
            }
            cache[i] = - (seq_t)i * cache[i - 1] + term_extra;
        }
        cached_to = n;
    }
    return cache[n];
}
