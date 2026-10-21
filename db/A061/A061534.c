/*
 * A061534  Expansion of (1-x^2)/(1-3*x-x^2+x^3).
 * https://oeis.org/A061534
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + a(n-2) - a(n-3) a(0) = 1, a(1) = 3, a(2) = 9,
 *            a(3) = 29 (OEIS A061534)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache      = NULL;   /* cached sequence values */
static idx_t cached_upto = (idx_t)-1; /* highest index computed so far */
static idx_t cache_size  = 0;      /* allocated array size */

/* grow the cache so that it can hold indices up to 'upto' (inclusive) */
static void grow_cache_to(idx_t upto)
{
    if (cache == NULL) {
        cache = (seq_t*)malloc((upto + 1) * sizeof(seq_t));
    } else if (cache_size <= upto) {
        cache = (seq_t*)realloc(cache, (upto + 1) * sizeof(seq_t));
    }
    if (cache == NULL) abort();
    if (upto + 1 > cache_size) cache_size = upto + 1;
}

/* sequence A061534 */
seq_t a(idx_t n)
{
    /* initialise base cases on first call or if n is smaller than cached range */
    if (cached_upto == (idx_t)-1 || n <= cached_upto) {
        /* ensure at least the first four terms are available */
        idx_t init_upto = (n > 3 ? n : 3);
        grow_cache_to(init_upto);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)9;
        cache[3] = (seq_t)29;
        cached_upto = 3;
        if (n <= cached_upto) return cache[n];
    } else {
        /* n > cached_upto, enlarge cache if necessary */
        if (n > cached_upto) {
            grow_cache_to(n);
            for (idx_t i = cached_upto + 1; i <= n; ++i) {
                cache[i] = cache[i-1] * 3 + cache[i-2] - cache[i-3];
            }
            cached_upto = n;
        }
    }
    return cache[n];
}
