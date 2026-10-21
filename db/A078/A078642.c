/*
 * A078642  Numbers with two representations as the sum of two Fibonacci
 *          numbers.
 * https://oeis.org/A078642
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2), a(1) = 4, a(2) = 6 * This follows from
 *            a(n) = 2·F(n+2) where F(1)=F(2)=1. * The function uses a simple
 *            memoising cache so that any call order is * efficient.
 *            Arithmetic is performed only with seq_t operators, so it * works
 *            for all supported integer sizes
 * Range:     u8 n <= 9, u16 n <= 21, u32 n <= 44, u64 n <= 90, u128 n <= 182,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Offset is 1; values for n==0 are not defined in OEIS, but we
       return 0 for safety. */
    if (n == 0) return (seq_t)0;

    /* Static cache: cache[i] holds a(i).  cache[0] is unused. */
    static seq_t *cache   = NULL;
    static idx_t cache_len = 0;      /* number of allocated slots (incl. 0) */

    /* Initialise cache if first call.  Allocate room for indices 0..3
       so that we have space for a(1), a(2), and the first computed
       a(3) when needed. */
    if (cache_len == 0) {
        cache_len = 4;                       /* slots 0,1,2,3   */
        cache = (seq_t *)calloc(cache_len, sizeof(seq_t));
        if (!cache) return (seq_t)0;         /* allocation failed */
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)6;
    }

    /* Grow cache if the requested index exceeds current allocation. */
    if (n >= cache_len) {
        idx_t new_len = (n + 1 > cache_len*2) ? (n + 1) : (cache_len * 2);
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return (seq_t)0;           /* allocation failed */
        /* Initialise newly allocated elements to 0. */
        for (idx_t i = cache_len; i < new_len; ++i) tmp[i] = 0;
        cache = tmp;
        cache_len = new_len;
    }

    /* Compute missing terms up to n using the recurrence. */
    for (idx_t i = 3; i <= n; ++i) {
        cache[i] = cache[i-1] + cache[i-2];
    }

    return cache[n];
}
