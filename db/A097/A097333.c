/*
 * A097333  a(n) = Sum_{k=0..n} C(n-k, floor(k/2)).
 * https://oeis.org/A097333
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 28, u32 n <= 57, u64 n <= 115,
 *            u128 n <= 232, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A097333
   Formula: a(0)=1, a(1)=2, a(2)=2, a(n)=a(n-1)+a(n-3) for n≥3
   Uses a static memoisation array that grows on demand.
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache        = NULL;   /* array of computed values   */
    static idx_t  cache_cap    = 0;      /* allocated capacity         */
    static idx_t  next_computed= 0;      /* first index not yet filled */

    /* enlarge the cache if necessary */
    if (cache_cap <= n) {
        idx_t new_cap = cache_cap ? cache_cap << 1 : 4;   /* at least 4 slots */
        if (new_cap <= n) new_cap = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)  /* allocation failure: return 0 as a safe default */
            return (seq_t)0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n using the recurrence */
    for (idx_t i = next_computed; i <= n; ++i) {
        if (i == 0)              cache[i] = (seq_t)1;
        else if (i == 1)          cache[i] = (seq_t)2;
        else if (i == 2)          cache[i] = (seq_t)2;
        else                      cache[i] = cache[i-1] + cache[i-3];
    }
    next_computed = n + 1;

    return cache[n];
}
