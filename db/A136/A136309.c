/*
 * A136309  a(n) = a(n-1) * 10^K + n + a(n-1); a(0) = 1; K = floor(log_10(n +
 *          a(n-1) + 1)).
 * https://oeis.org/A136309
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, less
 * Formula:   a(0) = 1 a(n) = a(n-1) * 10^K + n + a(n-1), where K =
 *            floor(log10(n + a(n-1) + 1))
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation memoises computed terms.  It uses the helper
   functions `pow10` and `digit_len` provided by the build.  A cache
   array of `seq_t` values is grown as needed.  Only operations that
   are defined on `seq_t` (addition, multiplication, comparison)
   are used. */

#include <stdlib.h>
#include <stdint.h>

#include "../utils/pow10.h"
#include "../utils/digit_len.h"

static seq_t *cache = NULL;     /* array of computed a(n) values */
static idx_t cache_size = 0;    /* number of cached values (valid indices 0..size-1) */
static size_t cache_cap = 0;    /* allocated capacity of cache array */

/* Helper to grow the cache to at least new_cap slots. */
static void grow_cache(size_t new_cap)
{
    if (new_cap <= cache_cap) return;
    while (cache_cap < new_cap) cache_cap = cache_cap ? cache_cap * 2 : 16;
    seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    cache = tmp;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* initialise cache with a(0)=1 */
    if (cache == NULL) {
        grow_cache(1);
        cache[0] = (seq_t)1;
        cache_size = 1;
    }

    /* if the requested term is already computed, return it */
    if (n < cache_size) return cache[n];

    /* otherwise compute terms up to n, extending the cache */
    grow_cache((size_t)n + 1);

    for (idx_t i = cache_size; i <= n; ++i) {
        /* compute K = floor(log10(i + a(i-1) + 1)) == digit_len(sum_plus)-1 */
        seq_t sum_plus = cache[i - 1];
        sum_plus += (seq_t)i;
        sum_plus += (seq_t)1;          /* sum_plus = a(i-1) + i + 1 */

        idx_t k = (idx_t)(digit_len(sum_plus) - 1);  /* K as idx_t */

        /* a(i) = a(i-1) * (10^K + 1) + i  */
        seq_t pow10k = pow10(k);
        seq_t mult = cache[i - 1] * (pow10k + (seq_t)1);
        mult += (seq_t)i;
        cache[i] = mult;
    }

    cache_size = n + 1;
    return cache[n];
}
