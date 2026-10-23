/*
 * A127804  a(2n) = 2^(2n), a(2n+1) = 2^(2n+1) + a(n).
 * https://oeis.org/A127804
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1 a(2n) = 2^(2n) = 2^(index) a(2n+1) = 2^(2n+1) + a(n)
 *            (index odd) (offset 0)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses memoisation to avoid recomputation and
   strictly follows the recurrence. */

#include <stdlib.h>
#include "../utils/pow2.h"

/* Forward declaration of the recursive helper. */
static seq_t compute_a(idx_t n);

/* Cache for memoisation.  cache[i] == 0 indicates that a(i) has not
   yet been computed.  The sequence never contains the value 0. */
static seq_t *cache = NULL;
static idx_t cache_cap = 0;

/* Ensure that the cache can store indices 0..n. */
static void ensure_cache(idx_t n)
{
    if (n < cache_cap) return;

    idx_t new_cap = cache_cap ? cache_cap : 1;
    while (new_cap <= n) new_cap <<= 1;

    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof (seq_t));
    cache = new_cache;
    /* initialise new entries to 0 (unknown) */
    for (idx_t i = cache_cap; i < new_cap; ++i) cache[i] = 0;
    cache_cap = new_cap;
}

/* Recursive computation with memoisation. */
static seq_t compute_a(idx_t n)
{
    if (n == 0) return 1;                 /* a(0) = 1 */
    ensure_cache(n);
    if (cache[n] != 0) return cache[n];

    seq_t result;
    if ((n & 1ULL) == 0) {                /* even index: a(n) = 2^n */
        result = pow2(n);
    } else {                              /* odd index   */
        idx_t k = n >> 1;                 /* n = 2k+1 -> k = floor(n/2) */
        result = pow2(n) + compute_a(k);
    }

    cache[n] = result;
    return result;
}

/* Public entry point required by the framework. */
seq_t a(idx_t n)
{
    return compute_a(n);
}
