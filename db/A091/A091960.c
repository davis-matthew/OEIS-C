/*
 * A091960  a(1)=1, a(2n)=a(2n-1)+(a(n)mod 2), a(2n+1)=a(2n)+1.
 * https://oeis.org/A091960
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2n)=a(2n-1)+(a(n) mod 2), a(2n+1)=a(2n)+1
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>     /* for realloc in C */

seq_t a(idx_t n)
{
    /* Static cache for memoisation   */
    static seq_t *cache     = NULL;  /* 1‑based array, cache[0] unused   */
    static idx_t  cache_cap = 0;     /* allocated size (including index 0) */
    static idx_t  cache_used = 0;    /* next index to compute (cache_valid up to cache_used‑1) */

    if (n < 1)                           /* sequence defined for n ≥ 1 */
        return (seq_t)0;

    /* Allocate initial cache if it doesn't exist yet   */
    if (cache == NULL) {
        cache_cap   = 2;                 /* need indices 0 and 1 */
        cache       = (seq_t *)realloc(NULL, cache_cap * sizeof(seq_t));
        if (cache == NULL)
            return (seq_t)0;             /* allocation failed */

        cache[1]    = (seq_t)1;          /* a(1) */
        cache_used  = 2;                 /* next index to compute */
    }

    /* Grow cache if we need more capacity   */
    if (n + 1 > cache_cap) {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL)
            return (seq_t)0;             /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute values from cache_used up to n   */
    for (idx_t i = cache_used; i <= n; ++i) {
        if ((i & 1) == 0) {             /* even: i = 2k      */
            idx_t k = i >> 1;
            cache[i] = cache[i - 1] + (cache[k] & (seq_t)1);
        } else {                        /* odd:  i = 2k+1    */
            cache[i] = cache[i - 1] + (seq_t)1;
        }
    }
    cache_used = n + 1;                 /* now cache[1..n] are valid */

    return cache[n];
}
