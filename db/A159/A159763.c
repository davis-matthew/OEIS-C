/*
 * A159763  Numerator of Hermite(n, 16/21).
 * https://oeis.org/A159763
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n+2) = 32*a(n+1) - 882*(n+1)*a(n), a(0) = 1, a(1) = 32
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 10, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = 21^n * Hermite(n,16/21) by the recurrence above. */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t cache_cap = 0;

/* Ensure the cache has space for indices up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (n < cache_cap) return;
    idx_t new_cap = cache_cap ? cache_cap : 1;
    while (new_cap <= n) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0U) return (seq_t)1;
    if (n == 1U) return (seq_t)32;

    ensure_cache(n);

    /* Initialize base values if cache is empty. */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)32;
        cache_len = 2;
    } else if (cache_len == 1) {
        cache[1] = (seq_t)32;
        cache_len = 2;
    }

    /* Compute values up to n using the recurrence. */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t term1 = (seq_t)32 * cache[i - 1];
        seq_t term2 = (seq_t)882 * (seq_t)(i - 1) * cache[i - 2];
        cache[i] = term1 - term2;
    }
    cache_len = n + 1;
    return cache[n];
}
