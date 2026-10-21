/*
 * A099365  Squares of A052918(n-1) (generalized Fibonacci).
 * https://oeis.org/A099365
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=1, a(2)=25, a(n)=26*a(n-1)+26*a(n-2)-a(n-3) for
 *            n>=3
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache      = NULL;
    static idx_t cache_len   = 0;   /* total allocated size   */
    static idx_t known       = 0;   /* number of terms stored */

    /* Ensure the cache is large enough for the three initial terms */
    if (cache_len < (idx_t)3) {
        idx_t new_len = 3;
        cache = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        cache_len = new_len;
    }

    /* Allocate more space if we need to reach index n */
    if (cache_len < n + (idx_t)1) {
        idx_t new_len = cache_len ? cache_len * 2 : (idx_t)3;
        if (new_len < n + (idx_t)1) new_len = n + (idx_t)1;
        cache = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        cache_len = new_len;
    }

    /* Initialise the first three terms once */
    if (known == (idx_t)0) {
        cache[(idx_t)0] = (seq_t)0;
        cache[(idx_t)1] = (seq_t)1;
        cache[(idx_t)2] = (seq_t)25;
        known = (idx_t)3;            /* gamma: terms 0..2 are known */
    }

    /* Compute any new terms up to index n */
    for (idx_t i = known; i <= n; ++i) {
        seq_t term = (cache[i-1] * (seq_t)26) + (cache[i-2] * (seq_t)26) - cache[i-3];
        cache[i] = term;
    }
    if (n + (idx_t)1 > known) known = n + (idx_t)1;

    return cache[n];
}
