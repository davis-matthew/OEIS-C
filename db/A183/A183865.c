/*
 * A183865  a(n) = n - 1 + ceiling(3*(n+2)/5); complement of A183864.
 * https://oeis.org/A183865
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 2*a(n-1) - a(n-2) + a(n-5) - 2*a(n-6) + a(n-7);
   Initial values: a(1)=1, a(2)=4, a(3)=8, a(4)=13, a(5)=19, a(6)=27, a(7)=36. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* first 7 terms, indexed 0..6 as a(1)..a(7) */
    static const seq_t init[7] = {
        (seq_t)1, (seq_t)4, (seq_t)8, (seq_t)13,
        (seq_t)19, (seq_t)27, (seq_t)36
    };

    /* static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of cached values */

    /* base case: n <= 7 */
    if (n <= 7) {
        return init[n - 1];
    }

    /* initialise cache if necessary */
    if (cache_len < 7) {
        cache = (seq_t *)realloc(cache, 7 * sizeof(seq_t));
        for (idx_t i = 0; i < 7; ++i) {
            cache[i] = init[i];
        }
        cache_len = 7;
    }

    /* grow cache up to n (indexes are 0‑based, value a(k) at cache[k-1]) */
    if (cache_len < n) {
        seq_t *tmp = (seq_t *)realloc(cache, n * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failure sentinel */
        cache = tmp;

        for (idx_t i = cache_len; i < n; ++i) {
            /* compute a(i+1) from previous values
               a(i+1) = 2*a(i) - a(i-1) + a(i-4) - 2*a(i-5) + a(i-6)   */
            cache[i] = 2 * cache[i - 1]
                       - cache[i - 2]
                       + cache[i - 5]
                       - 2 * cache[i - 6]
                       + cache[i - 7];
        }
        cache_len = n;
    }

    return cache[n - 1];
}
