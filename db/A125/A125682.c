/*
 * A125682  a(n) = 3*(6^n - 1)/5.
 * https://oeis.org/A125682
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*(6^n - 1)/5. This can be computed by the recurrence:
 *            a(1) = 3 a(n) = 6*a(n-1) + 3 for n ≥ 2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* cache[1] holds a(1) */
static idx_t  cache_max = 0;  /* highest index cached */

static void
ensure_cache(idx_t n)
{
    if (n == 0) return;

    if (cache == NULL) {
        /* allocate space for indices 0..n (index 0 unused) */
        cache = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        cache[1] = (seq_t)3;
        cache_max = 1;
    }

    if (n > cache_max) {
        /* grow the cache to hold up to n */
        cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        for (idx_t i = cache_max + 1; i <= n; ++i) {
            cache[i] = (seq_t)6 * cache[i - 1] + (seq_t)3;
        }
        cache_max = n;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;  /* defined only for n ≥ 1, but return 0 for 0 */

    ensure_cache(n);
    return cache[n];
}
