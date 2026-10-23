/*
 * A167682  Expansion of (1 - 2*x + 5*x^2) / (1 - 3*x)^2.
 * https://oeis.org/A167682
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 17, u64 n <= 37, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=4, a(2)=20, a(n)=6*a(n-1)-9*a(n-2) for n>2 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;        /* memoised values */
    static idx_t cache_len = 0;        /* number of cached terms */

    /* Ensure cache has at least 3 elements so that the base cases exist. */
    if (cache_len < 3) {
        idx_t new_len = 3;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        /* initialise the missing base cases */
        if (cache_len == 0) {
            cache[0] = (seq_t)1;
            cache[1] = (seq_t)4;
            cache[2] = (seq_t)20;
        } else if (cache_len == 1) {
            cache[1] = (seq_t)4;
            cache[2] = (seq_t)20;
        } else { /* cache_len == 2 */
            cache[2] = (seq_t)20;
        }
        cache_len = new_len;
    }

    /* If the requested index is already cached, return it. */
    if (n < 3) return cache[n];

    /* Grow cache if necessary and compute missing terms via the recurrence. */
    if (n + 1 > cache_len) {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        for (idx_t i = cache_len; i <= n; ++i) {
            cache[i] = (seq_t)6 * cache[i - 1] - (seq_t)9 * cache[i - 2];
        }
        cache_len = new_len;
    }

    return cache[n];
}
