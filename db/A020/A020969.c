/*
 * A020969  Expansion of 1/((1-7*x)*(1-8*x)*(1-12*x)).
 * https://oeis.org/A020969
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 27*a(n-1) - 236*a(n-2) + 672*a(n-3) (n>=3) a(0)=1,
 *            a(1)=27, a(2)=493 Uses lazy memoization and O(n) time for new
 *            maximum n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;  /* number of stored elements */

    /* Initialise base values once */
    if (cache_size == 0) {
        cache_size = 3;  /* indices 0,1,2 */
        cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        if (!cache) abort();                      /* memory failure */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)27;
        cache[2] = (seq_t)493;
    }

    /* Return immediately if already computed */
    if (n < cache_size)
        return cache[n];

    /* Expand cached array up to n */
    size_t old_len = cache_size;
    size_t new_len = (size_t)(n + 1);
    seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;

    /* Compute missing terms */
    for (idx_t i = old_len; i <= n; ++i) {
        seq_t val;
        if (i == 0) {
            val = (seq_t)1;
        } else if (i == 1) {
            val = (seq_t)27;
        } else if (i == 2) {
            val = (seq_t)493;
        } else {
            /* recurrence: a(i) = 27*a(i-1) - 236*a(i-2) + 672*a(i-3) */
            val = (seq_t)27 * cache[i-1]
                - (seq_t)236 * cache[i-2]
                + (seq_t)672 * cache[i-3];
        }
        cache[i] = val;
    }

    cache_size = new_len;
    return cache[n];
}
