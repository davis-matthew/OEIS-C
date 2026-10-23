/*
 * A173659  a(n) = a(n-1) + 8*a(n-2) - 3*a(n-3) - 17*a(n-4) + a(n-5) +
 *          13*a(n-6) + 4*a(n-7) - 6*a(n-8) - a(n-9).
 * https://oeis.org/A173659
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = a(n-1) + 8*a(n-2) - 3*a(n-3) - 17*a(n-4) + a(n-5) +
 *            13*a(n-6) + 4*a(n-7) - 6*a(n-8) - a(n-9) a(0) = 0, a(1) = 0,
 *            a(2) = 0, a(3) = 1, a(4) = 4, a(5) = 9, a(6) = 33, a(7) = 79,
 *            a(8) = 250
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 24, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static const seq_t init_vals[9] = {0, 0, 0, 1, 4, 9, 33, 79, 250};

static seq_t *cache   = NULL;   /* cached sequence values */
static size_t cap     = 0;      /* current allocated capacity */
static idx_t  max_n   = (idx_t)-1; /* largest n already computed */

/* Ensure the cache has space for at least n+1 elements */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        cap = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        for (size_t i = 0; i < 9; ++i) cache[i] = init_vals[i];
        max_n = 8;
    }
    if ((size_t)n < cap) return;
    size_t newcap = cap;
    while ((size_t)n >= newcap) newcap <<= 1;
    cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    cap = newcap;
}

/* Compute a(n) using the recurrence and cache */
seq_t a(idx_t n)
{
    if (n < 9) return init_vals[n];

    ensure_cache(n);

    /* Compute terms up to n if not already done */
    while (max_n < n) {
        idx_t i = max_n + 1;

        /* Use 128‑bit intermediates to avoid overflow on 64‑bit builds */
        unsigned __int128 pos = (unsigned __int128)cache[i-1];
        pos += (unsigned __int128)cache[i-2] * 8u;
        pos += (unsigned __int128)cache[i-5];
        pos += (unsigned __int128)cache[i-6] * 13u;
        pos += (unsigned __int128)cache[i-7] * 4u;

        unsigned __int128 neg = (unsigned __int128)cache[i-3] * 3u;
        neg += (unsigned __int128)cache[i-4] * 17u;
        neg += (unsigned __int128)cache[i-8] * 6u;
        neg += (unsigned __int128)cache[i-9];

        cache[i] = (seq_t)(pos - neg);
        max_n = i;
    }

    return cache[n];
}
