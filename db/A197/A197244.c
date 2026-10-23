/*
 * A197244  Number of n X 2 0..4 arrays with each element x equal to the
 *          number its horizontal and vertical neighbors equal to 0,0,1,1,1
 *          for x=0,1,2,3,4.
 * https://oeis.org/A197244
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-2) + 2*a(n-3) + a(n-4) - 2*a(n-5) for n >= 6
 *            Initial terms: a(1)=2, a(2)=4, a(3)=8, a(4)=16, a(5)=40
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 28, u64 n <= 56, u128 n <= 111,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;    /* array of computed terms */
    static size_t cache_cap = 0;   /* allocated capacity in terms   */
    static size_t cache_len = 0;   /* number of terms currently stored */

    /* n must be >= 1 for the sequence; if not, return 0 */
    if (n < 1)
        return (seq_t)0;

    /* ensure we have at least the first five values cached */
    if (cache_len == 0) {
        cache_cap = 5;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) return (seq_t)0;  /* allocation failed; return 0 */
        cache[0] = (seq_t)2;   /* a(1) */
        cache[1] = (seq_t)4;   /* a(2) */
        cache[2] = (seq_t)8;   /* a(3) */
        cache[3] = (seq_t)16;  /* a(4) */
        cache[4] = (seq_t)40;  /* a(5) */
        cache_len = 5;
    }

    /* grow cache if necessary */
    if ((idx_t)cache_cap < n) {
        size_t new_cap = n;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp) return (seq_t)0;  /* allocation failed; return 0 */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* if the requested term is already computed, return it */
    if (n <= (idx_t)cache_len)
        return cache[n - 1];

    /* compute missing terms up to n using the recurrence */
    for (idx_t i = (idx_t)cache_len + 1; i <= n; ++i) {
        seq_t val =
            (seq_t)4 * cache[i - 3]  /* 4*a(i-2)   */
          + (seq_t)2 * cache[i - 4]  /* 2*a(i-3)   */
          +      cache[i - 5]        /*   a(i-4)   */
          - (seq_t)2 * cache[i - 6]; /* -2*a(i-5)  */
        cache[i - 1] = val;
    }
    cache_len = (size_t)n;
    return cache[n - 1];
}
