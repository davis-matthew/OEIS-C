/*
 * A025924  Expansion of 1/((1-x^9)*(1-x^10)*(1-x^12)).
 * https://oeis.org/A025924
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-9) + a(n-10) + a(n-12) - a(n-19) - a(n-21) - a(n-22)
 *            + a(n-31) with a(0) = 1 and a(n) = 0 for 1 ≤ n < 9
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* array holding computed values   */
static size_t cache_cap = 0;     /* current capacity of cache         */
static idx_t last_computed = 0;  /* highest index that has been filled */

/* Double the capacity of the cache until it can hold `needed` elements. */
static void grow_cache(size_t needed)
{
    if (cache_cap >= needed)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 1;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *newp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!newp)          /* allocation failure – abort the program */
        abort();
    cache = newp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Make sure the cache can hold the requested index. */
    grow_cache((size_t)n + 1);

    /* Initialise a(0).  It is 1 and has never been written yet. */
    if (last_computed == 0 && cache[0] == 0)
        cache[0] = 1;

    /* If the value has already been computed, return it. */
    if (n <= last_computed)
        return cache[n];

    /* Compute all missing values up to n. */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t val = 0;
        if (i >= 9)  val += cache[i - 9];
        if (i >= 10) val += cache[i - 10];
        if (i >= 12) val += cache[i - 12];
        if (i >= 19) val -= cache[i - 19];
        if (i >= 21) val -= cache[i - 21];
        if (i >= 22) val -= cache[i - 22];
        if (i >= 31) val += cache[i - 31];
        cache[i] = val;
    }
    last_computed = n;
    return cache[n];
}
