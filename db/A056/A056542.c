/*
 * A056542  a(n) = n*a(n-1) + 1, a(1) = 0.
 * https://oeis.org/A056542
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*a(n-1) + 1, a(1) = 0 (offset 1)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Lazy memoization of the recurrence.
     * cache[i] holds a(i+1)  (since a(1) -> cache[0]).
     */
    static seq_t *cache = NULL;      /* dynamic array of computed terms */
    static idx_t cache_cnt = 0;      /* number of terms currently stored */

    /* initialise cache for a(1) */
    if (cache == NULL) {
        cache = (seq_t*)malloc(sizeof(seq_t));
        if (cache == NULL) { /* allocation failure – abort safely */
            abort();
        }
        cache[0] = (seq_t)0;   /* a(1) = 0 */
        cache_cnt = 1;
    }

    /* extend cache up to n if necessary */
    for (idx_t k = cache_cnt + 1; k <= n; ++k) {
        /* compute a(k) = k * a(k-1) + 1 */
        seq_t val = (seq_t)k * cache[k - 2] + (seq_t)1;

        /* grow the cache by one element */
        seq_t *newptr = (seq_t*)realloc(cache, k * sizeof(seq_t));
        if (newptr == NULL) {
            abort();  /* allocation failure – fail fast */
        }
        cache = newptr;
        cache[k - 1] = val;
    }
    cache_cnt = n;

    /* n>=1 always under the specification */
    return cache[n - 1];
}
