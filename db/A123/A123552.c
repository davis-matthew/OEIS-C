/*
 * A123552  Expansion of 1/(1 - x - x^3 + x^5).
 * https://oeis.org/A123552
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 53, u32 n <= 108, u64 n <= 220,
 *            u128 n <= 442, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = a(n-1) + a(n-3) - a(n-5)  for n>=5
 * with a(0)=a(1)=a(2)=1, a(3)=2, a(4)=3
 * This uses a simple memoised recurrence that can be evaluated in O(n).
 */

#include <stdlib.h>

/* static cache for memoisation */
static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* allocated capacity (number of elements) */
static idx_t  cache_len = 0;   /* length of valid data in cache */

/* ensure the cache has enough capacity to hold elements up to index n */
static void ensure_capacity(idx_t n)
{
    size_t needed = (size_t)n + 1;          /* indices 0..n inclusive */
    if (cache_cap >= needed)
        return;
    size_t new_cap = cache_cap == 0 ? 8 : cache_cap;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* compute the sequence up to index n (inclusive), filling the cache */
static void compute_to(idx_t n)
{
    if (n < cache_len)
        return;  /* already computed */

    ensure_capacity(n);

    /* initialise base values on first use */
    if (cache_len == 0) {
        cache[0] = 1;
        cache[1] = 1;
        cache[2] = 1;
        cache[3] = 2;
        cache[4] = 3;
        cache_len = 5;  /* elements 0..4 are now valid */
    }

    /* apply recurrence for i = cache_len .. n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t prev1 = cache[(size_t)(i - 1)];
        seq_t prev3 = cache[(size_t)(i - 3)];
        seq_t prev5 = cache[(size_t)(i - 5)];
        cache[(size_t)i] = prev1 + prev3 - prev5;
    }

    cache_len = n + 1;  /* update length after computation */
}

/* public sequence function */
seq_t a(idx_t n)
{
    /* base cases handled directly */
    if (n <= 4) {
        static const seq_t base[5] = {1, 1, 1, 2, 3};
        return base[(size_t)n];
    }

    /* compute up to n if needed */
    compute_to(n);
    return cache[(size_t)n];
}
