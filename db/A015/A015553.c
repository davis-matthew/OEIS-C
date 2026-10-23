/*
 * A015553  Expansion of x/(1 - 6*x - 11*x^2).
 * https://oeis.org/A015553
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) + 11*a(n-2); a(0)=0, a(1)=1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;  /* number of elements stored (cache_len-1 is max index) */

static void ensure_cache(idx_t n)
{
    if (n < cache_len) return;               /* already computed */

    idx_t new_len = n + 1;
    if (new_len < 2) new_len = 2;             /* at least for a(0) and a(1) */

#ifdef __cplusplus
    /* Allocate a new array and copy the old values if any */
    seq_t *new_cache = new seq_t[new_len];
    if (cache != NULL) {
        for (idx_t i = 0; i < cache_len; ++i) new_cache[i] = cache[i];
        delete[] cache;
    }
    cache = new_cache;
    cache_len = new_len;
#else
    /* Allocate or grow the array with realloc */
    seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!new_cache) return;                  /* failure – give up */
    cache = new_cache;
    cache_len = new_len;
#endif

    /* Initialise base values if we are building the table from scratch */
    if (cache_len == 2) {
        cache[0] = 0;
        cache[1] = 1;
        /* No further values to compute when n < 2 */
        return;
    }

    /* Compute any new entries that are now required */
    for (idx_t i = 2; i <= n; ++i) {
        cache[i] = 6 * cache[i - 1] + 11 * cache[i - 2];
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
