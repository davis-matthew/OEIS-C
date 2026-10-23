/*
 * A189444  Number of n X 4 array permutations with each element moving zero
 *          or one space horizontally or diagonally.
 * https://oeis.org/A189444
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A189444: Number of n X 4 array permutations with each element
 * moving zero or one space horizontally or diagonally.
 *
 * Empirical recurrence (order 5):
 *   a(n) = 12*a(n-1) - 12*a(n-2) + 12*a(n-3) - 12*a(n-4) + a(n-5)
 * Initial values:
 *   a(1) = 5,  a(2) = 61,  a(3) = 666,  a(4) = 7300,  a(5) = 80282
 *
 * The implementation keeps a static cache expanded as needed and
 * computes further terms on first request.
 */

#include <stdlib.h>

/* static cache */
static seq_t *cache = NULL;
static size_t cache_len = 0;  /* number of terms stored */
static size_t cache_cap = 0;  /* allocated capacity */

/* Ensure the cache contains terms up to n (1‑based) */
static void ensure_cache(idx_t n)
{
    if (n <= cache_len) return;          /* already computed */

    size_t target = (size_t)n;

    /* Allocate or grow the cache */
    if (cache == NULL) {
        cache_cap = target > 8 ? target : 8;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_len = 0;
    } else if (cache_cap < target) {
        size_t new_cap = cache_cap;
        while (new_cap < target) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Compute missing terms */
    for (size_t i = cache_len; i < target; ++i) {
        seq_t val;
        uint64_t idx = (uint64_t)(i + 1);  /* 1-based index */

        if (idx == 1)      val = (seq_t)5;
        else if (idx == 2) val = (seq_t)61;
        else if (idx == 3) val = (seq_t)666;
        else if (idx == 4) val = (seq_t)7300;
        else if (idx == 5) val = (seq_t)80282;
        else {
            /* a(n) = 12*a(n-1) - 12*a(n-2) + 12*a(n-3) - 12*a(n-4) + a(n-5) */
            seq_t a1 = cache[i-1]; /* a(n-1) */
            seq_t a2 = cache[i-2]; /* a(n-2) */
            seq_t a3 = cache[i-3]; /* a(n-3) */
            seq_t a4 = cache[i-4]; /* a(n-4) */
            seq_t a5 = cache[i-5]; /* a(n-5) */

            seq_t t = a1 * (seq_t)12;
            t = t - (a2 * (seq_t)12);
            t = t + (a3 * (seq_t)12);
            t = t - (a4 * (seq_t)12);
            t = t + a5;
            val = t;
        }

        cache[i] = val;
    }

    cache_len = target;
}

/* Public interface */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined, but safe */
    ensure_cache(n);
    return cache[(size_t)(n-1)];
}
