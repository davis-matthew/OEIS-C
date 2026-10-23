/*
 * A020951  a(2n+1)=a(n), a(2n)=a(n)+a(n-1).
 * https://oeis.org/A020951
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A020951: a(0) = 1, a(1) = 0,
 * a(2n)   = a(n) + a(n-1),
 * a(2n+1) = a(n)
 */
#include <stdlib.h>

static seq_t *cache = NULL;     /* array holding computed values */
static idx_t cache_nmax = 0;    /* highest index currently cached (inclusive) */

/* Extend cache up to index n inclusive, computing any missing values */
static void extend_cache(idx_t n)
{
    if (cache == NULL) {
        /* First allocation */
        cache = (seq_t *)malloc(sizeof(seq_t) * ((size_t)n + 1));
        if (cache == NULL) return;   /* allocation failed */
        for (idx_t i = 0; i <= n; ++i) {
            if (i == 0) {
                cache[0] = 1;
            } else if (i == 1) {
                cache[1] = 0;
            } else if (i & 1) {        /* i odd: 2k+1 */
                cache[i] = cache[(i - 1) >> 1];
            } else {                  /* i even: 2k   */
                idx_t k = i >> 1;
                cache[i] = cache[k] + cache[k - 1];
            }
        }
        cache_nmax = n;
    } else if (n > cache_nmax) {
        /* Grow the array */
        seq_t *tmp = (seq_t *)realloc(cache,
                                      sizeof(seq_t) * ((size_t)n + 1));
        if (tmp == NULL) return;   /* reallocation failed */
        cache = tmp;
        for (idx_t i = cache_nmax + 1; i <= n; ++i) {
            if (i == 0) {
                cache[0] = 1;
            } else if (i == 1) {
                cache[1] = 0;
            } else if (i & 1) {        /* i odd: 2k+1 */
                cache[i] = cache[(i - 1) >> 1];
            } else {                  /* i even: 2k   */
                idx_t k = i >> 1;
                cache[i] = cache[k] + cache[k - 1];
            }
        }
        cache_nmax = n;
    }
    /* If n <= cache_nmax, nothing to do */
}

seq_t a(idx_t n)
{
    if (cache == NULL || n > cache_nmax) {
        extend_cache(n);
    }
    /* If allocation failed earlier, return 0 as a sane fallback */
    return (cache != NULL) ? cache[(size_t)n] : 0;
}
