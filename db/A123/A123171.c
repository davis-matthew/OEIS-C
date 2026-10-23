/*
 * A123171  a(1) = 123, a(n) = sum of digits of all previous terms.
 * https://oeis.org/A123171
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1) = 123; a(n) = sum of digits of all previous terms (n ≥ 2).
 *            For n ≥ 3 this is equivalent to a(n) = a(n‑1) +
 *            digit_sum(a(n‑1))
 * Range:     u8 n <= 32, u16 n <= 3280, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Internal helper: sum of decimal digits of a value of type seq_t. */
static inline seq_t digit_sum_internal(seq_t x)
{
    seq_t sum = 0;
    while (x > 0) {
        sum += x % 10;
        x /= 10;
    }
    return sum;
}

/* ---- Memoisation storage ---- */
static seq_t *seq_cache      = NULL;   /* holds a(1)..a(cached_len) */
static idx_t  cached_len     = 0;       /* number of terms currently cached */
static size_t cache_cap      = 0;       /* allocated array capacity         */

/* ---- Main sequence function ---- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for offset 1, safe return */

    /* Initialise cache on first call: store a(1) and a(2) */
    if (cached_len == 0) {
        cache_cap = 2;
        seq_cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!seq_cache) abort();
        seq_cache[0] = (seq_t)123;                     /* a(1) */
        seq_cache[1] = digit_sum_internal((seq_t)123); /* a(2) */
        cached_len = 2;
    }

    /* If requested index is already computed, return it */
    if (n <= cached_len) {
        return seq_cache[(size_t)(n - 1)];
    }

    /* Ensure the array has enough capacity for up to n terms */
    size_t need = (size_t)n;
    if (need > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 1;
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        seq_cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute terms from (cached_len + 1) up to n */
    for (idx_t i = cached_len + 1; i <= n; ++i) {
        seq_t prev = seq_cache[(size_t)(i - 2)];      /* a(i‑1) */
        seq_t term = prev + digit_sum_internal(prev);/* a(i) for i≥3 */
        seq_cache[(size_t)(i - 1)] = term;
    }
    cached_len = n;

    return seq_cache[(size_t)(n - 1)];
}
