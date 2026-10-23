/*
 * A022028  Define the sequence T(a(0),a(1)) by a(n+2) is the greatest integer
 *          such that a(n+2)/a(n+1) < a(n+1)/a(n) for n >= 0. This is T(2,32).
 * https://oeis.org/A022028
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 16*a(n-1) - 8*a(n-3) (n >= 3, with a(0)=2, a(1)=32,
 *            a(2)=511)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoization */
    static seq_t *cache = NULL;      /* array of computed terms */
    static idx_t cache_cap = 0;      /* current capacity of the array   */
    static idx_t cache_len = 0;      /* number of valid entries          */

    /* Initial allocation and first three terms */
    if (!cache) {
        cache_cap = 3;
        cache_len = 3;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;                   /* allocation failure */
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)32;
        cache[2] = (seq_t)511;
    }

    /* Return already known term */
    if (n < cache_len) {
        return cache[n];
    }

    /* Expand cache if needed */
    if ((idx_t)(n + 1) > cache_cap) {
        idx_t new_cap = (idx_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n using the recurrence */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* For i >= 3 recurrence holds; first three terms already set */
        seq_t t1 = cache[i - 1];
        t1 *= (seq_t)16;                     /* 16 * a(i-1) */

        seq_t t2 = cache[i - 3];
        t2 *= (seq_t)8;                      /* 8  * a(i-3) */

        cache[i] = t1 - t2;                  /* a(i) = 16*a(i-1) - 8*a(i-3) */
    }
    cache_len = n + 1;

    return cache[n];
}
