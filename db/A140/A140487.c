/*
 * A140487  Trajectory of 3 under repeated application of the map: n -> n +
 *          second-smallest number that does not divide n.
 * https://oeis.org/A140487
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A140487:
   a(1) = 3;
   For n > 1: a(n) = a(n-1) + k, where k is the second smallest
   positive integer that does NOT divide a(n-1).  This implements
   the map n → n + second‑smallest non‑divisor of n. */
#include <stdlib.h>

static seq_t *cache = NULL;       /* memoized sequence values */
static size_t cache_len = 0;      /* number of values stored */
static size_t cache_cap = 0;      /* allocated capacity */

/* Ensure the cache can hold at least new_cap elements. */
static void
ensure_capacity(size_t new_cap)
{
    if (new_cap <= cache_cap)
        return;
    size_t new_cap2 = new_cap > 0 ? new_cap : 1;
    while (new_cap2 < new_cap)
        new_cap2 <<= 1;          /* grow by factor of two */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap2 * sizeof(seq_t));
    if (!tmp) abort();          /* out of memory */
    cache = tmp;
    cache_cap = new_cap2;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* undefined for offset 0, return 0 */

    /* Return memoized value if already computed. */
    if ((size_t)n <= cache_len)
        return cache[n - 1];

    /* Compute missing terms up to index n. */
    ensure_capacity((size_t)n);
    if (cache_len == 0) {
        cache[0] = (seq_t)3;     /* a(1) */
        cache_len = 1;
    }

    for (size_t i = cache_len; i < (size_t)n; ++i) {
        seq_t prev = cache[i - 1];
        /* Find second smallest non‑divisor of prev. */
        size_t cnt = 0;
        idx_t k = 2;
        for (;;) {
            if (prev % (seq_t)k != 0) {
                ++cnt;
                if (cnt == 2)
                    break;
            }
            ++k;
        }
        cache[i] = prev + (seq_t)k;
    }
    cache_len = (size_t)n;
    return cache[n - 1];
}
