/*
 * A017905  Expansion of 1/(1 - x^11 - x^12 - ...).
 * https://oeis.org/A017905
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 82, u32 n <= 147, u64 n <= 278,
 *            u128 n <= 541, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A017905 – Number of compositions of n with all parts ≥ 11
 *
 *  Recurrence:  a(0) = 1
 *               a(k) = 0          for 1 ≤ k ≤ 10
 *               a(n) = a(n-1) + a(n-11)   for n ≥ 11
 *
 *  The function memoises the values, so subsequent calls are O(1)
 *  for already‑computed indices and O(n) for the first call to a
 *  large n.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static memoisation cache */
    static seq_t *cache    = NULL;   /* holds a(0)…a(current_max)          */
    static idx_t cache_len = 0;      /* number of stored values (max n+1)   */

    /* initialise cache on first call */
    if (cache_len == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (cache == NULL) abort();          /* allocation failed              */
        cache[0] = (seq_t)1;                 /* a(0) = 1                       */
        cache_len = 1;                       /* we know a(0)                   */
    }

    /* extend the cache if the requested index is beyond what we have */
    if (n >= cache_len) {
        /* realloc to hold indices 0 … n */
        seq_t *new_cache = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        if (new_cache == NULL) abort();      /* allocation failed              */
        cache = new_cache;

        /* compute the missing terms starting from the previous length */
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i <= 10) {
                /* a(k)=0 for 1≤k≤10 */
                cache[i] = (seq_t)0;
            } else {
                /* recurrence a(i)=a(i-1)+a(i-11) */
                cache[i] = cache[i - 1] + cache[i - 11];
            }
        }
        cache_len = n + 1;   /* we now know values up to a(n)             */
    }

    return cache[n];
}
