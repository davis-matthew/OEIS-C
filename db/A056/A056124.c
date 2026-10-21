/*
 * A056124  a(n) = 3*a(n-1) - a(n-2) + 8 with a(0)=1, a(1)=11.
 * https://oeis.org/A056124
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*a(n-1) - a(n-2) + 8, a(0)=1, a(1)=11
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 21, u64 n <= 44, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* dynamic array of computed terms     */
static idx_t cache_size = 0;         /* current capacity (in seq_t slots)   */
static idx_t computed_max = (idx_t)-1; /* index of the largest computed term */

/* Ensure the cache can hold indices 0 … n.
   Capacity is doubled until it exceeds n+1. */
static void ensure_cache(idx_t n)
{
    if (cache && cache_size > n)
        return;                    /* already large enough */

    size_t new_cap = cache_size ? cache_size * 2 : 4;
    while (new_cap <= n)
        new_cap <<= 1;             /* multiply by 2 until adequate */

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_size = new_cap;
}

/* Compute a(n) using the linear recurrence with memoisation. */
seq_t a(idx_t n)
{
    /* Initialise the first two terms on first call. */
    if (cache == NULL)
    {
        ensure_cache(n);              /* allocate enough space for the current n */
        cache[0] = (seq_t)1;          /* a(0) = 1    */
        cache[1] = (seq_t)11;         /* a(1) = 11   */
        computed_max = 1;             /* we have computed up to index 1 */
    }

    /* If the requested index is already computed, just return it. */
    if (n <= computed_max)
        return cache[n];

    /* Make sure the cache is large enough for all indices up to n. */
    ensure_cache(n);

    /* Compute terms from (computed_max + 1) up to n inclusively. */
    for (idx_t i = computed_max + 1; i <= n; ++i)
    {
        /* a(i) = 3*a(i-1) - a(i-2) + 8 */
        seq_t term = cache[i - 1];
        term = term * ((seq_t)3);     /* 3 * a(i-1) */
        term = term - cache[i - 2];   /* subtract a(i-2) */
        term = term + ((seq_t)8);     /* add constant 8 */
        cache[i] = term;
    }

    computed_max = n;
    return cache[n];
}
