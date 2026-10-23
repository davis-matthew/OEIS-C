/*
 * A161661  a(1)=1. a(n) = smallest multiple of {number of divisors of a(n-1)}
 *          that is >= n.
 * https://oeis.org/A161661
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 240, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1; for n>1,
   a(n)=ceil(n / d) * d where d=divisor_count(a(n-1)). */
#include <stddef.h>
#include <stdlib.h>

/* Count positive divisors of n using trial division. */
static seq_t divisor_count(seq_t n)
{
    seq_t result = 1;
    seq_t temp = n;
    seq_t d = 2;
    while (d * d <= temp) {
        if (temp % d == 0) {
            seq_t exp = 0;
            while (temp % d == 0) {
                temp /= d;
                exp += 1;
            }
            result *= (exp + 1);
        }
        d += 1;
    }
    if (temp > 1) {
        result *= 2;
    }
    return result;
}

/* Memoization cache for a(n). */
static seq_t *cache = NULL;
static idx_t cache_size = 0; /* number of terms stored (1‑based) */
static idx_t cache_cap = 0;  /* allocated capacity */

/* Ensure cache can store at least `needed` elements. */
static void grow_cache(idx_t needed)
{
    if (needed <= cache_cap) return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 1;
    while (new_cap < needed) new_cap *= 2;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined below the offset */

    /* Allocate cache big enough. */
    grow_cache(n);

    /* Compute missing terms lazily. */
    if (cache_size < n) {
        if (cache_size == 0) {
            cache[0] = (seq_t)1;   /* a(1) */
            cache_size = 1;
        }
        for (idx_t i = cache_size; i < n; ++i) {
            seq_t prev = cache[i - 1];
            seq_t d = divisor_count(prev);

            /* Compute ceil((i+1)/d) * d. */
            idx_t idx_val = i + 1;          /* 1‑based index */
            idx_t k = (idx_val + (idx_t)d - 1) / (idx_t)d; /* ceil division */
            cache[i] = (seq_t)(k * (idx_t)d);
        }
        cache_size = n;
    }
    return cache[n - 1];
}
