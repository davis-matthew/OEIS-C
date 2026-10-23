/*
 * A016201  Expansion of g.f. 1/((1-x)*(1-2*x)*(1-7*x)).
 * https://oeis.org/A016201
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (49*7^n - 24*2^n + 5)/30
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence used: a(0)=1, a(1)=10, a(2)=77,
   a(n)=10*a(n-1) - 23*a(n-2) + 14*a(n-3)  for n>=3 */
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the first three terms explicitly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)10;
    if (n == 2) return (seq_t)77;

    /* Static cache for memoisation */
    static seq_t *cache = NULL;          /* array of computed values   */
    static size_t cap   = 0;             /* current allocation size    */
    static size_t len   = 0;             /* number of values already filled (next index to compute) */

    /* If the requested value is already cached, return it */
    if (cache && (size_t)n < len)
        return cache[(size_t)n];

    /* Initialise cache on first use */
    if (cache == NULL) {
        /* Ensure space for at least the first three values and growth up to n */
        cap = 3;
        while (cap <= (size_t)n) cap <<= 1;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)10;
        cache[2] = (seq_t)77;
        len = 3;
    }
    else if ((size_t)n >= cap) {
        /* Grow the cache to accommodate n+1 elements */
        size_t newcap = cap;
        while (newcap <= (size_t)n) newcap <<= 1;
        cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        cap = newcap;
    }

    /* Compute missing terms up to n (inclusive) */
    for (size_t i = len; i <= (size_t)n; ++i) {
        seq_t v1 = cache[i-1];
        seq_t v2 = cache[i-2];
        seq_t v3 = cache[i-3];
        seq_t val = 10 * v1;         /* 10*a(n-1)   */
        val -= 23 * v2;              /* -23*a(n-2)  */
        val += 14 * v3;              /* +14*a(n-3)  */
        cache[i] = val;
    }
    len = (size_t)n + 1;

    return cache[(size_t)n];
}
