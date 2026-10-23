/*
 * A145117  Numbers of length n binary words with fewer than 9 0-digits
 *          between any pair of consecutive 1-digits.
 * https://oeis.org/A145117
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) counts binary words of length n having fewer than * 9 zeros
 *            between any pair of consecutive 1’s. * Recurrence for n ≥ 11: *
 *            a(n) = 3*a(n-1) – 2*a(n-2) – a(n-10) + a(n-11) * Initial values
 *            (OEIS offset 0): * a(0)=1, a(1)=2, a(2)=4, … , a(10)=1024
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow2.h"   /* pow2(idx_t exp) returns 2^exp as seq_t */

static seq_t *cache = NULL;      /* computed a(n) values */
static size_t   cache_len = 0;   /* number of entries in cache */

/* Ensure cache contains values up to index n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (cache_len > n) return;           /* already computed */

    /* enlarge cache: new capacity at least n+1, double each time */
    size_t new_cap = cache_len ? cache_len : 1;
    while (new_cap <= (size_t)n) new_cap <<= 1;

    cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!cache) { /* memory allocation failed */
        /* In production we might abort or handle error; here we simply set 0. */
        return;
    }

    /* compute missing values */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i <= 10) {                                 /* 2^i for i <= 10 */
            cache[i] = pow2(i);
        } else {
            /* recurrence: a(n) = 3*a(n-1) - 2*a(n-2) - a(n-10) + a(n-11) */
            seq_t term1 = 3 * cache[i-1];
            seq_t term2 = 2 * cache[i-2];
            seq_t term3 = cache[i-10];
            seq_t term4 = cache[i-11];
            cache[i] = term1 - term2 - term3 + term4;
        }
    }
    cache_len = n + 1;
}

seq_t a(idx_t n)
{
    /* Ensure the cache contains our requested value. */
    ensure_cache(n);
    return cache[n];
}
