/*
 * A215076  a(n) = 3*a(n-1) + 4*a(n-2) + a(n-3) with a(0)=3, a(1)=3, a(2)=17.
 * https://oeis.org/A215076
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 4*a(n-2) + a(n-3); Initial values: a(0)=3,
 *            a(1)=3, a(2)=17
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* memoised terms */
static idx_t cache_cap = 0;      /* allocated array size */
static idx_t last_computed = 0;  /* index of the last computed term */

/* Ensure that the cache can hold at least indices 0 … n. */
static void ensure_capacity(idx_t n)
{
    if (n < cache_cap) return;
    idx_t new_cap = (n + 1) > 2 * cache_cap ? (n + 1) : 2 * cache_cap;
    if (new_cap < 3) new_cap = 3;                /* at least for first 3 terms */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* Compute a(n) using the recurrence, memoising all intermediate terms. */
seq_t a(idx_t n)
{
    /* Lazy initialisation of the first three terms. */
    if (cache == NULL) {
        ensure_capacity(2);                 /* needs space for indices 0..2 */
        cache[0] = 3;
        cache[1] = 3;
        cache[2] = 17;
        last_computed = 2;
    }

    if (n <= last_computed) return cache[n];

    ensure_capacity(n);

    for (idx_t i = last_computed + 1; i <= n; ++i) {
        if (i == 0)       cache[i] = 3;
        else if (i == 1)  cache[i] = 3;
        else if (i == 2)  cache[i] = 17;
        else {
            cache[i] = 3 * cache[i - 1] + 4 * cache[i - 2] + cache[i - 3];
        }
    }

    last_computed = n;
    return cache[n];
}
