/*
 * A178632  a(n) = 45*((10^n - 1)/9)^2.
 * https://oeis.org/A178632
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 45 * ((10^n - 1) / 9)^2 * Recurrence: a(n) = 111*a(n-1) -
 *            1110*a(n-2) + 1000*a(n-3), n > 3 * Initial terms: a(1) = 45,
 *            a(2) = 5445, a(3) = 554445
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* behaviour below offset is irrelevant */

    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0; /* allocated capacity */
    static idx_t cache_len = 0; /* number of terms actually stored */

    /* First call: make sure cache holds the first three terms. */
    if (cache_len == 0) {
        /* Allocate as much as needed for the first request or at least 3. */
        cache_cap = (n > 3 ? n : 3);
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        /* Initialise with base values */
        cache[0] = (seq_t)45;          /* a(1) */
        cache[1] = (seq_t)5445;        /* a(2) */
        cache[2] = (seq_t)554445;      /* a(3) */
        cache_len = 3;
    }
    /* If the requested n exceeds current capacity, enlarge the cache. */
    else if (n > cache_cap) {
        cache_cap = n;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }

    /* Return already computed value if available. */
    if (n <= cache_len) return cache[n - 1];

    /* Compute terms up to n using the recurrence. */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        /* i-1 is the index in 0‑based array for a(i). */
        seq_t val = 111 * cache[i - 2] - 1110 * cache[i - 3] + 1000 * cache[i - 4];
        cache[i - 1] = val;
    }
    cache_len = n;
    return cache[n - 1];
}
