/*
 * A052657  Expansion of e.g.f. x^2/((1-x)^2*(1+x)).
 * https://oeis.org/A052657
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n!*floor(n/2) (OEIS A052657)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          a(0) = 0, a(1) = 0, a(n) = n!.floor(n/2) for n ≥ 2  */

#include <stdlib.h>

static seq_t *cache = NULL;          /* cache of computed a(i) */
static idx_t cache_size = 0;        /* number of entries filled so far   */
static idx_t cache_capacity = 0;    /* allocated size of cache array     */
static seq_t fact = 1;              /* factorial of the last cached n   */
static idx_t last_cached = 0;       /* index of the last cached factorial */

static void ensure_cache(idx_t n)
{
    /* initialise cache on first use */
    if (cache == NULL) {
        cache_capacity = 4;
        cache = (seq_t *)malloc(cache_capacity * sizeof(seq_t));
        /* a(0) = 0 */
        cache[0] = 0;
        cache_size = 1;
        last_cached = 0;
        fact = 1;                   /* 0! = 1 */
    }

    /* already computed? */
    if (n < cache_size)
        return;

    /* grow the cache array if necessary */
    if (n + 1 > cache_capacity) {
        idx_t new_cap = cache_capacity ? cache_capacity : 4;
        while (new_cap <= n + 1)
            new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_capacity = new_cap;
    }

    /* compute missing values up to n */
    for (idx_t k = last_cached + 1; k <= n; ++k) {
        fact = fact * (seq_t)k;           /* fact = k!  */
        seq_t mult = (seq_t)(k / 2);       /* floor(k/2) */
        cache[k] = fact * mult;           /* a(k) = k!*floor(k/2) */
    }

    cache_size = n + 1;
    last_cached = n;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
