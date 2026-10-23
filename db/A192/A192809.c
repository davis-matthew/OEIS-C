/*
 * A192809  Coefficient of x in the reduction of the polynomial (x^2 + 2)^n by
 *          x^3 -> x^2 + 2.
 * https://oeis.org/A192809
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) - 12*a(n-2) + 8*a(n-3)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 29, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Initial values: a(0)=0, a(1)=0, a(2)=2                      */

#include <stdlib.h>

/* Cache for already computed terms. */
static seq_t *cache = NULL;          /* array of terms */
static size_t cache_cap = 0;          /* number of allocated slots */
static idx_t highest_compiled = 0;    /* largest n whose value is in cache */

/* Make sure cache can hold indices 0..n (inclusive). */
static void ensure_cache_capacity(idx_t n)
{
    if (cache_cap > (size_t)n)
        return;

    size_t new_cap = cache_cap > 0 ? cache_cap * 2 : 4;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;

    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)                     /* allocation failed – keep old cache */
        return;

    /* Zero only the newly added slots. */
    size_t old_cap = cache_cap;
    cache = new_cache;
    cache_cap = new_cap;
    for (size_t i = old_cap; i < new_cap; ++i)
        cache[i] = 0;
}

/* Return a(n) according to the recurrence, with memoisation. */
seq_t a(idx_t n)
{
    ensure_cache_capacity(n);

    /* Initialise the base terms once. */
    if (highest_compiled < 2) {
        cache[0] = 0;
        cache[1] = 0;
        cache[2] = 2;
        highest_compiled = 2;
    }

    /* If we already have the value, return it immediately. */
    if (n <= highest_compiled)
        return cache[n];

    /* Compute any missing terms up to n.  We start at max(3, highest_compiled+1)
       because the recurrence is defined only for n ≥ 3. */
    idx_t start = highest_compiled + 1;
    if (start < 3)
        start = 3;

    for (idx_t i = start; i <= n; ++i) {
        seq_t t1 = cache[i - 1];
        seq_t t2 = cache[i - 2];
        seq_t t3 = cache[i - 3];
        cache[i] = 7 * t1 - 12 * t2 + 8 * t3;
    }
    highest_compiled = n;
    return cache[n];
}
