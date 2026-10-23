/*
 * A167033  a(n) = 20*a(n-1) - 64*a(n-2) + 3 for n > 1; a(0) = 1, a(1) = 22.
 * https://oeis.org/A167033
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 20*a(n-1) - 64*a(n-2) + 3 for n > 1; a(0) = 1, a(1) = 22
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;

/* Ensure cache holds values up to index n. */
static void ensure_cache(idx_t n)
{
    if (n < cache_len)                     /* already computed   */
        return;

    /* Grow the array.  The cast is required for C++ compilers. */
    seq_t *new_cache = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
    if (!new_cache)                        /* allocation failed   */
        abort();

    cache = new_cache;

    /* Set base values if the cache was empty or had only a(0). */
    if (cache_len == 0) {                  /* first time          */
        cache[0] = (seq_t)1;
        if (n >= 1)                       /* keep a(1)  if needed */
            cache[1] = (seq_t)22;
    } else if (cache_len == 1) {           /* only a(0) present  */
        if (n >= 1)
            cache[1] = (seq_t)22;
    }

    /* Continue the recurrence from the existing length. */
    idx_t start = (cache_len > 2) ? cache_len : 2;
    for (idx_t i = start; i <= n; ++i)
        cache[i] = (seq_t)20 * cache[i - 1] - (seq_t)64 * cache[i - 2] + (seq_t)3;

    cache_len = n + 1;                     /* new length          */
}

/* Public function: returns a(n). */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
