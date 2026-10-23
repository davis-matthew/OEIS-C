/*
 * A115059  a(n+4) = a(n+3)+a(n+1)+a(n)+k(n), where k(n) = 0, 1, 0, or -1
 *          according to n mod 4.
 * https://oeis.org/A115059
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n+4) = a(n+3) + a(n+1) + a(n) + k(n) where k(n) = 0, 1, 0, -1
 *            for n ≡ 0, 1, 2, 3 (mod 4). Initial values: a(0) = 0, a(1) = 0,
 *            a(2) = 1, a(3) = 1
 * Range:     u8 n <= 14, u16 n <= 26, u32 n <= 49, u64 n <= 95, u128 n <= 187,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;          /* memoisation array */
static size_t cache_cap = 0;          /* allocated capacity */
static idx_t  last = 3;               /* highest index computed so far */

/* Ensure the cache contains values up to index n */
static void ensure_cache(idx_t n)
{
    /* Allocate or grow the cache to hold indices up to n */
    if (cache == NULL) {
        /* Initial allocation: enough for indices 0 .. max(3, n) */
        size_t init_cap = 4;
        while (init_cap <= (size_t)n + 1)
            init_cap <<= 1;
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (!cache) abort();           /* simple error handling */

        /* Initialise the first four terms */
        cache[0] = 0;
        cache[1] = 0;
        cache[2] = 1;
        cache[3] = 1;
        cache_cap = init_cap;
        last = 3;
    } else if ((size_t)n + 1 > cache_cap) {
        /* Grow the cache exponentially */
        size_t new_cap = cache_cap;
        while ((size_t)n + 1 > new_cap)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute terms up to index n if not already present */
    if (last < n) {
        for (idx_t i = last + 1; i <= n; ++i) {
            /* a(i) = a(i-1) + a(i-3) + a(i-4) + k(i-4) */
            seq_t sum = cache[(size_t)(i - 1)] +
                        cache[(size_t)(i - 3)] +
                        cache[(size_t)(i - 4)];
            switch ((i - 4) & 3) {        /* (i-4) mod 4 */
                case 1: sum += (seq_t)1;  break;
                case 3: sum -= (seq_t)1;  break; /* unsigned subtraction */
                default: break;           /* 0 and 2 add nothing */
            }
            cache[(size_t)i] = sum;
        }
        last = n;
    }
}

/* Public function required by the platform */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
