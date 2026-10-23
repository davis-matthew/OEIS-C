/*
 * A017998  Expansion of 1/((1-3x)*(1-7x)*(1-9x)).
 * https://oeis.org/A017998
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=19, a(2)=250; a(n)=19*a(n-1)-111*a(n-2)+189*a(n-3)
 *            for n>=3
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* compute OEIS A017998 – coefficient sequence of
   1/((1-3x)*(1-7x)*(1-9x))                     */
seq_t a(idx_t n)
{
    /* static cache to store already computed values */
    static seq_t *cache   = NULL;  /* array of sequence values   */
    static size_t cache_cap = 0;   /* allocated capacity          */
    static size_t cache_len = 0;   /* number of valid entries     */

    /* enlarge cache if necessary (c++ requires explicit cast) */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = ((size_t)n + 1) < 8 ? 8
                                  : ((size_t)n + 1) * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise the first three known terms if they are not yet set */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache_len = 1;
    }
    if (cache_len <= 1 && n >= 1) {
        cache[1] = (seq_t)19;
        cache_len = 2;
    }
    if (cache_len <= 2 && n >= 2) {
        cache[2] = (seq_t)250;
        cache_len = 3;
    }

    /* compute missing terms up to the required index n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val = (seq_t)19  * cache[(size_t)(i - 1)]
                  - (seq_t)111 * cache[(size_t)(i - 2)]
                  + (seq_t)189 * cache[(size_t)(i - 3)];
        cache[(size_t)i] = val;
    }

    /* adjust cache_len only if we have newly added entries */
    size_t new_len = (size_t)n + 1;
    if (new_len > cache_len) {
        cache_len = new_len;
    }

    return cache[(size_t)n];
}
