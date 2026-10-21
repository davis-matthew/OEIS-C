/*
 * A096658  a(n) = (2^n)*a(n-1) + (2^(n-1))*a(n-2), a(0)=1, a(1)=2.
 * https://oeis.org/A096658
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2^n)*a(n-1) + (2^(n-1))*a(n-2), a(0)=1, a(1)=2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_capacity = 0;

/* Ensure that cache can hold indices up to up_to (inclusive). */
static void ensure_cache(size_t up_to)
{
    if (up_to < cache_capacity)
        return;

    size_t new_cap = cache_capacity ? cache_capacity * 2 : 16;
    if (up_to + 1 > new_cap)
        new_cap = up_to + 1;

    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        abort(); /* fatal error: not enough memory */

    /* Initialize newly allocated entries to 0 */
    for (size_t i = cache_capacity; i < new_cap; ++i)
        new_cache[i] = (seq_t)0;

    cache = new_cache;
    cache_capacity = new_cap;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Handle the base cases. */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)2;

    /* Make sure we have cached up to n. */
    ensure_cache(n);

    /* If already computed, return it. */
    if (cache[n] != (seq_t)0)
        return cache[n];

    /* Compute iteratively up to n. */
    /* First ensure base values in cache. */
    if (cache[0] == (seq_t)0)
        cache[0] = (seq_t)1;
    if (cache[1] == (seq_t)0)
        cache[1] = (seq_t)2;

    for (idx_t k = 2; k <= n; ++k)
    {
        if (cache[k] != (seq_t)0)
            continue; /* already computed */

        seq_t pow_k   = ((seq_t)1) << k;   /* 2^k  */
        seq_t pow_km1 = ((seq_t)1) << (k-1); /* 2^(k-1) */

        cache[k] = pow_k   * cache[k-1] + pow_km1 * cache[k-2];
    }

    return cache[n];
}
