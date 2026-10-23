/*
 * A128720  Number of paths in the first quadrant from (0,0) to (n,0) using
 *          steps U=(1,1), D=(1,-1), h=(1,0) and H=(2,0).
 * https://oeis.org/A128720
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2) + sum_{j=0}^{n-2} a(j) * a(n-2-j), a(0) =
 *            1, a(1) = 1 This recurrence follows from a(n) = a(n-1) + a(n-2)
 *            + c(n-2) where c(k) = sum_{j=0}^{k} a(j) * a(k-j)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 22, u64 n <= 41, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <assert.h>

static seq_t *a_cache = NULL;     /* cache of previously computed values */
static idx_t  cache_size = 0;     /* current allocated capacity */
static idx_t  cache_limit = 0;    /* number of values already computed */

/* Grow the cache to at least `need+1` elements (indices 0..need).  */
static void
grow_cache(idx_t need)
{
    if (need < cache_size) return;
    size_t new_cap = cache_size ? cache_size * 2 : 16;
    while (new_cap <= (size_t)need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                 /* allocation failed */
    a_cache = tmp;
    cache_size = new_cap;
}

/* Ensure that a_cache[n] is computed.  */
static void
ensure_a(idx_t n)
{
    if (n < cache_limit) return;       /* already computed */

    grow_cache(n);

    /* Initialise first two values if needed.  */
    if (cache_limit == 0) {
        a_cache[0] = (seq_t)1;
        cache_limit = 1;
    }
    if (cache_limit == 1) {
        a_cache[1] = (seq_t)1;
        cache_limit = 2;
    }

    for (idx_t m = cache_limit; m <= n; ++m) {
        seq_t conv = 0;                /* sum_{j=0}^{m-2} a(j) * a(m-2-j) */
        for (idx_t j = 0; j <= m - 2; ++j) {
            conv += a_cache[j] * a_cache[m - 2 - j];
        }
        a_cache[m] = a_cache[m - 1] + a_cache[m - 2] + conv;
    }
    cache_limit = n + 1;
}

seq_t a(idx_t n)
{
    ensure_a(n);
    return a_cache[n];
}
