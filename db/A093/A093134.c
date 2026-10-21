/*
 * A093134  A Jacobsthal trisection.
 * https://oeis.org/A093134
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (8^n + 8*(-1)^n)/9 * Recurrence (for n ≥ 2): a(n) =
 *            7*a(n‑1) + 8*a(n‑2) * Initial values: a(0) = 1, a(1) = 0 * The
 *            routine memoises the sequence in a static array so that *
 *            repeated calls are O(1) after the required index has first been
 *            * computed. The code uses only integer arithmetic and...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for malloc, realloc */

static seq_t *cache = NULL;   /* memoised values                */
static idx_t  cache_len = 0;  /* number of stored values        */
static idx_t  cache_alloc = 0;/* allocated array length         */

seq_t a(idx_t n)
{
    /* initialise with first two terms */
    if (cache_alloc == 0) {
        cache_alloc = 2;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_alloc);
        if (!cache) return (seq_t)0;          /* graceful failure */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache_len = 2;
    }

    /* grow the array if needed */
    if (n + (idx_t)1 > cache_alloc) {
        idx_t new_alloc = cache_alloc;
        while (new_alloc < n + (idx_t)1)
            new_alloc = (new_alloc > 0) ? new_alloc * 2 : 2;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_alloc);
        if (!tmp) return (seq_t)0;          /* graceful failure */
        cache = tmp;
        cache_alloc = new_alloc;
    }

    /* compute missing terms */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;
        } else if (i == 1) {
            cache[1] = (seq_t)0;
        } else {
            cache[i] = (seq_t)7 * cache[i - 1] + (seq_t)8 * cache[i - 2];
        }
    }
    cache_len = n + (idx_t)1;
    return cache[n];
}
