/*
 * A096231  Number of n-th generation triangles in the tiling of the
 *          hyperbolic plane by triangles with angles {Pi/2, Pi/3, 0}.
 * https://oeis.org/A096231
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 35, u32 n <= 75, u64 n <= 153,
 *            u128 n <= 311, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=3, a(2)=5, a(3)=7, a(4)=9, a(5)=12, a(6)=16
   a(n) = a(n-1) + a(n-5)   for n > 6
*/

#include <stdlib.h>   /* realloc */
#include <stdint.h>   /* uint64_t */

static const seq_t _base[7] = {1, 3, 5, 7, 9, 12, 16};

seq_t a(idx_t n)
{
    /* cache for memoisation */
    static seq_t *cache     = NULL; /* array of already computed terms   */
    static idx_t  cache_len = 0;    /* allocated length                 */
    static idx_t  last_computed = 6;/* highest index that has been computed */
    static int    inited = 0;       /* have we filled the base entries?  */

    /* direct return for the first seven terms */
    if (n < 7)
        return _base[(size_t)n];

    /* ensure the cache can hold indices up to n */
    idx_t needed = n + 1;
    if (cache_len < needed) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp)   /* allocation failed; return 0 as a defensive sentinel */
            return 0;
        cache = tmp;
        cache_len = needed;
    }

    /* initialise the first seven entries the first time we touch the cache */
    if (!inited) {
        for (idx_t i = 0; i < 7 && i < cache_len; ++i)
            cache[i] = _base[i];
        inited = 1;
    }

    /* compute any missing values up to n */
    if (last_computed < n) {
        for (idx_t i = last_computed + 1; i <= n; ++i)
            cache[i] = cache[i - 1] + cache[i - 5];
        last_computed = n;
    }

    return cache[n];
}
