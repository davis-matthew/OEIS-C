/*
 * A081072  a(n) = Fibonacci(4*n) + 3, or Fibonacci(2*n+2)*Lucas(2*n-2).
 * https://oeis.org/A081072
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(4*n) + 3 Recurrence: a(0) = 3, a(1) = 6, a(2) =
 *            24, a(n) = 8*a(n-1) - 8*a(n-2) + a(n-3) for n >= 3
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute the n-th term of OEIS A081072.
 * The function is memoized: previous results are cached, so the
 * cost of a call is O(n - max_cached_n).
 */
seq_t a(idx_t n)
{
    /* static cache for computed values */
    static seq_t *cache = NULL;      /* pointer to the cache array   */
    static size_t       cap   = 0;   /* current allocated size       */
    static idx_t        max_n = (idx_t)-1;  /* highest index cached */

    /* Initialise cache on first call */
    if (cache == NULL) {
        cap = 3; /* we will store terms 0,1,2 */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache[0] = 3;   /* a(0)  */
        cache[1] = 6;   /* a(1)  */
        cache[2] = 24;  /* a(2)  */
        max_n = 2;
    }

    /* Return cached term if already computed */
    if (n <= max_n)
        return cache[n];

    /* Extend cache to hold terms up to n */
    size_t new_cap = (size_t)(n + 1);
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (new_cache == NULL) { /* allocation failure – fall back to plain loop */
        seq_t a0 = 3, a1 = 6, a2 = 24, an;
        if (n == 0) return a0;
        if (n == 1) return a1;
        if (n == 2) return a2;
        for (idx_t i = 3; i <= n; ++i) {
            an = a1 * 8;
            an -= a0 * 8;
            an += a2;
            a0 = a1; a1 = a2; a2 = an;
        }
        return an;
    }

    cache = new_cache;
    cap   = new_cap;

    /* Compute missing terms using the recurrence */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        seq_t term = cache[i-1] * 8;   /* 8*a(n-1)   */
        term -= cache[i-2] * 8;        /* minus 8*a(n-2) */
        term += cache[i-3];            /* plus a(n-3)     */
        cache[i] = term;
    }
    max_n = n;
    return cache[n];
}
