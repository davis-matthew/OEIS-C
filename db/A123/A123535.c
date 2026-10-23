/*
 * A123535  Recurrence from values at floor of a third and two-thirds.
 * https://oeis.org/A123535
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 1 a(n) = a(floor(n/3)) + a(floor(2*n/3)) + n + 1 for n >
 *            0
 * Range:     u8 n <= 35, u16 n <= 4373, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Memoisation cache: cache[i] holds a(i) for 0 ≤ i ≤ max_computed */
static seq_t *cache = NULL;
static idx_t cache_limit = 0;     /* allocated size */
static idx_t max_computed = 0;    /* largest n that has been computed */

/* Ensure that indices 0 … n are addressable.  No initialisation of
   newly allocated entries is performed; they are set as needed by the
   algorithm. */
static void ensure_cache(idx_t n)
{
    if (n + 1 <= cache_limit)
        return;
    idx_t new_limit = cache_limit ? cache_limit * 2 : 1;
    while (new_limit <= n)
        new_limit <<= 1;              /* double until large enough */
    seq_t *tmp = (seq_t *)realloc(cache, new_limit * sizeof(seq_t));
    if (!tmp)
        exit(EXIT_FAILURE);           /* out of memory */
    cache = tmp;
    cache_limit = new_limit;
}

/* Compute all values up to n (inclusive) and store them in the cache. */
static void compute_up_to(idx_t n)
{
    /* Make sure the cache exists and contains the base value. */
    ensure_cache(n);
    cache[0] = (seq_t)1;                /* a(0) = 1 */

    /* Nothing to do if we have already computed up to n. */
    if (n <= max_computed)
        return;

    /* Compute values in increasing order so that lower indices are
       available when needed. */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        idx_t f1 = i / 3;              /* floor(i/3)   */
        idx_t f2 = (2 * i) / 3;        /* floor(2*i/3) */
        cache[i] = cache[f1] + cache[f2] + (seq_t)i + 1;
    }
    max_computed = n;
}

/* Public function required by the problem statement. */
seq_t a(idx_t n)
{
    compute_up_to(n);
    return cache[n];
}
