/*
 * A041104  Numerators of continued fraction convergents to sqrt(60).
 * https://oeis.org/A041104
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = numerator of the n-th convergent of sqrt(60) *
 *            Recurrence: a(-1) = 1, a(0) = 7 * a(n) = c(n) * a(n‑1) + a(n‑2)
 *            for n ≥ 1 * where c(0)=7 and for n ≥ 1 the continued‑fraction
 *            terms repeat * 1, 2, 1, 14
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 19, u64 n <= 41, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;     /* memoised numerators                 */
static idx_t cache_len   = 0;    /* number of elements currently stored  */
static idx_t cache_cap   = 0;    /* allocated capacity (in seq_t units)  */

seq_t a(idx_t n)
{
    /* Ensure the cache can hold the requested index */
    if (n + 1 > cache_cap) {
        idx_t new_cap = (cache_cap > 0) ? cache_cap * 2 : 2;
        if (new_cap < n + 1)
            new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* unlikely in test harness          */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise the first two numerators on first use */
    if (cache_len == 0) {
        cache[0] = (seq_t)7;          /* a(0) */
        cache[1] = (seq_t)8;          /* a(1) = 1*7 + 1 */
        cache_len = 2;
    }

    /* Extend the cache up to the requested index */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* Compute the continued‑fraction term c(i) */
        seq_t ci;
        if (i == 0) {
            ci = (seq_t)7;
        } else {
            idx_t k = (i - 1) % 4;
            switch (k) {
                case 0: ci = (seq_t)1; break; /* 1 */
                case 1: ci = (seq_t)2; break; /* 2 */
                case 2: ci = (seq_t)1; break; /* 1 */
                default: ci = (seq_t)14; break; /* 14 */
            }
        }
        if (i >= 2) {
            cache[i] = ci * cache[i - 1] + cache[i - 2];
        }
    }

    cache_len = n + 1;
    return cache[n];
}
