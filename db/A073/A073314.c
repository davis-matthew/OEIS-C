/*
 * A073314  Binomial transform, alternating in sign, of Lucas generalized
 *          numbers S(n): S(n) = S(n-1) + S(n-2) + S(n-3), S(0)=3, S(1)=1,
 *          S(2)=3.
 * https://oeis.org/A073314
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 2*a(n-1) - 2*a(n-3), a(0)=3, a(1)=2, a(2)=4
 * Range:     i8 n <= 12, i16 n <= 22, i32 n <= 51, i64 n <= 99, i128 n <= 201,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes A073314 by memoising earlier results.  */
#include <stdlib.h>   /* malloc, realloc, abort */

seq_t a(idx_t n)
{
    /* static cache for already computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;      /* allocated length of cache */
    static idx_t cache_max = (idx_t)-1; /* largest index that has been computed */

    /* initial allocation and base values */
    if (cache == NULL) {
        size_t init = 3;               /* need at least three entries */
        if (n + 1 > init)
            init = n + 1;
        cache = (seq_t *)malloc(sizeof(seq_t) * init);
        if (!cache) abort();           /* allocation failure */

        cache[0] = (seq_t)3;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)4;

        cache_len = init;
        cache_max = 2;
    }

    /* enlarge the cache if required */
    if (n + 1 > cache_len) {
        size_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_len);
        if (!tmp) abort();
        cache = tmp;
        cache_len = new_len;
    }

    /* compute any missing terms up to n */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t t1 = cache[i - 1];
        seq_t t3 = cache[i - 3];
        cache[i] = t1 + t1 - t3 - t3;   /* 2*a(i-1) - 2*a(i-3) */
    }
    if (n > cache_max)
        cache_max = n;

    return cache[n];
}
