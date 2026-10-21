/*
 * A087171  Expansion of (1 + 5*x)/(1 + 9*x + 25*x^2).
 * https://oeis.org/A087171
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 13, i64 n <= 23, i128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1, a(1) = -4; for n ≥ 2, a(n) = -9*a(n-1) - 25*a(n-2) */

#include <stdlib.h>

static seq_t *cache = NULL;      /* memoised values of a(n) */
static idx_t cache_cap = 0;      /* allocated capacity of cache */
static idx_t cache_len = 0;      /* number of valid entries in cache */

/* Ensure the cache can hold at least 'cap' entries. */
static void ensure_cache_cap(idx_t cap) {
    if (cap <= cache_cap)
        return;
    seq_t *newp = (seq_t *)realloc(cache, cap * sizeof(seq_t));
    if (!newp)
        abort();                 /* memory allocation failed */
    cache = newp;
    cache_cap = cap;
}

seq_t a(idx_t n) {
    /* Initialise base cases on the first call. */
    if (cache_len == 0) {
        ensure_cache_cap(2);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)-4;
        cache_len = 2;
    }

    /* Return the value if it has already been computed. */
    if (n < cache_len)
        return cache[n];

    /* Compute values up to n and extend the cache. */
    ensure_cache_cap(n + 1);
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t t1 = (seq_t)9 * cache[i - 1];
        seq_t t2 = (seq_t)25 * cache[i - 2];
        cache[i] = -t1 - t2;
    }
    cache_len = n + 1;
    return cache[n];
}
