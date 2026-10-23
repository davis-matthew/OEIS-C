/*
 * A049873  a(n) = Sum{a(k): k=0,1,2,...,n-4,n-2,n-1}; a(n-3) is not a
 *          summand; initial terms are 2,2,3.
 * https://oeis.org/A049873
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 2*a(n-1) - a(n-3) + a(n-4),  a(0)=2, a(1)=2, a(2)=3, a(3)=5
 * (equivalent to the sum over all earlier terms except a(n-3))
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cap = 0;   /* number of allocated elements in cache */

    /* handle smallest indices directly */
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)5;

    /* initialise cache if it does not exist yet */
    if (cap == 0) {
        cap = 4;                                 /* holds indices 0..3 */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;                    /* allocation failed – return 0 */
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)5;
    }

    /* expand cache to at least n+1 elements, if necessary */
    if (n >= cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                     /* realloc failed – return 0 */
        cache = tmp;
        /* compute new terms */
        for (idx_t i = (idx_t)cap; i <= n; ++i) {
            /* recurrence uses only indices that are already defined */
            cache[i] = cache[i - 1] + cache[i - 1] - cache[i - 3] + cache[i - 4];
        }
        cap = new_cap;
    }

    return cache[n];
}
