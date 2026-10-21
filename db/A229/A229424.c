/*
 * A229424  Number of n X 4 0..2 arrays with horizontal differences mod 3
 *          never 1, vertical differences mod 3 never -1, rows
 *          lexicographically nondecreasing, and columns lexicographically
 *          nonincreasing.
 * https://oeis.org/A229424
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) - 36*a(n-2) + 84*a(n-3) - 126*a(n-4) +
 *            126*a(n-5) * - 84*a(n-6) + 36*a(n-7) - 9*a(n-8) + a(n-9) for n >
 *            9 * Initial values: a(1)=12, a(2)=55, a(3)=222, a(4)=754,
 *            a(5)=2204, * a(6)=5700, a(7)=13345, a(8)=28794, a(9)=58053
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 45, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mul_small.h"

static seq_t *cache   = NULL;   /* array of computed terms, index 0 unused */
static size_t cache_cap = 0;     /* allocated size of cache */
static idx_t  computed_max = 0;  /* largest index that has been computed */

/* Ensure the cache can hold terms up to index n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (n + 1 <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    if (new_cap <= (size_t)n)
        new_cap = (size_t)n + 1;

    if (cache == NULL) {
        cache = (seq_t *)calloc(new_cap, sizeof(seq_t));
        cache_cap = new_cap;
    } else {
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp) {
            cache = tmp;
            /* zero newly allocated elements */
            for (size_t i = cache_cap; i < new_cap; ++i)
                cache[i] = (seq_t)0;
            cache_cap = new_cap;
        }
    }
}

/* The known initial terms, index 0 unused. */
static const seq_t init[10] = {
    (seq_t)0,
    (seq_t)12,
    (seq_t)55,
    (seq_t)222,
    (seq_t)754,
    (seq_t)2204,
    (seq_t)5700,
    (seq_t)13345,
    (seq_t)28794,
    (seq_t)58053
};

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0; /* undefined for offset 0, but safe default */

    if (n > computed_max) {
        ensure_cache(n);

        /* Fill initial values first if not already done */
        idx_t start = computed_max + 1;
        if (start <= 9) {
            for (idx_t k = start; k <= 9 && k <= n; ++k)
                cache[(size_t)k] = init[(size_t)k];
            start = 10;
        }

        /* Compute subsequent terms using the recurrence relation */
        for (idx_t k = start; k <= n; ++k) {
            /* Positive contributions */
            seq_t res = mul_small(cache[(size_t)(k - 1)], 9);
            res += mul_small(cache[(size_t)(k - 3)], 84);
            res += mul_small(cache[(size_t)(k - 5)], 126);
            res += mul_small(cache[(size_t)(k - 7)], 36);
            res += cache[(size_t)(k - 9)];
            /* Negative contributions */
            res -= mul_small(cache[(size_t)(k - 2)], 36);
            res -= mul_small(cache[(size_t)(k - 4)], 126);
            res -= mul_small(cache[(size_t)(k - 6)], 84);
            res -= mul_small(cache[(size_t)(k - 8)], 9);
            cache[(size_t)k] = res;
        }
        computed_max = n;
    }

    return cache[(size_t)n];
}
