/*
 * A009632  Expansion of sinh(x)/cosh(log(1+x)).
 * https://oeis.org/A009632
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = ( (n%2==0 ? 2*n : 2) - 2*n*a(n-1) - n*(n-1)*a(n-2) ) / 2
 *            with a(0) = 0, a(1) = 1
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 14, i64 n <= 21, i128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Compute the OEIS sequence A009632.
 * The values satisfy        2 a(n) = rhs - 2 n a(n-1) - n(n-1) a(n-2)
 * where rhs = 2                     if n is odd
 *            2 n                   if n is even
 * with a(0)=0, a(1)=1.
 *
 * The implementation memoizes all terms that have been computed
 * using a static array.  The array is grown as necessary.
 */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* memoization array */
    static size_t cache_len = 0;  /* number of stored terms */

    /* initialise cache if it is the first call */
    if (cache_len == 0)
    {
        cache_len = 2;                   /* need at least a[0] and a[1] */
        cache = (seq_t*)malloc(cache_len * sizeof(seq_t));
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
    }

    /* grow the cache if the requested index is beyond the current size */
    if ((size_t)n >= cache_len)
    {
        size_t old_len = cache_len;
        cache_len = (size_t)(n + 1);
        cache = (seq_t*)realloc(cache, cache_len * sizeof(seq_t));

        for (idx_t i = (idx_t)old_len; i <= n; ++i)
        {
            seq_t rhs = ((i & 1) == 0) ? (seq_t)2 * (seq_t)i : (seq_t)2;

            /* compute 2 * i * a(i-1)  and  i * (i-1) * a(i-2)   */
            seq_t term1 = ((seq_t)2 * (seq_t)i) * cache[i - 1];
            seq_t term2 = ((seq_t)i * (seq_t)(i - 1)) * cache[i - 2];

            /* a(i) = (rhs - term1 - term2) / 2   */
            cache[i] = (rhs - term1 - term2) / (seq_t)2;
        }
    }

    return cache[n];
}
