/*
 * A090390  Repeatedly multiply (1,0,0) by ([1,2,2],[2,1,2],[2,2,3]); sequence
 *          gives leading entry.
 * https://oeis.org/A090390
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 5*a(n-1) + 5*a(n-2) - a(n-3); a(0)=1, a(1)=1, a(2)=9
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to avoid recomputation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t max_computed = 0;

    /* base cases handled directly for speed */
    if (n < 3) {
        return (n == 0 || n == 1) ? (seq_t)1 : (seq_t)9;
    }

    /* initialise cache if necessary */
    if (cache == NULL) {
        cache_size = 4;                     /* enough for indices 0..3 */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return (seq_t)0;        /* allocation failure */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)9;
        max_computed = 2;
    }

    /* grow cache if request exceeds current maximum index */
    if (n >= cache_size) {
        idx_t new_size = (n + 1) * 2;        /* exponential growth */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failure */
        cache = tmp;
        cache_size = new_size;
    }

    /* if already computed, return cached value */
    if (n <= max_computed) {
        return cache[n];
    }

    /* compute missing terms up to n and store in cache */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        cache[i] = (seq_t)5 * cache[i - 1]
                 + (seq_t)5 * cache[i - 2]
                 - cache[i - 3];
    }
    max_computed = n;
    return cache[n];
}
