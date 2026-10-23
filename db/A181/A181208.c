/*
 * A181208  Number of n X 4 binary matrices with no two 1's adjacent
 *          diagonally or antidiagonally.
 * https://oeis.org/A181208
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) + 8*a(n-2) - 48*a(n-3) + 24*a(n-4) + 32*a(n-5) -
 *            16*a(n-6) * Initial values: a(1)=16, a(2)=64, a(3)=484,
 *            a(4)=2704, a(5)=17424, a(6)=104976
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 24, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_vals[6] = {16, 64, 484, 2704, 17424, 104976};

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t cache_cap = 0;

/* Ensure the cache can hold at least n elements (indices 1..n) */
static void ensure_capacity(idx_t n)
{
    if (n <= cache_cap)
        return;
    idx_t new_cap = cache_cap ? cache_cap : 8;
    while (new_cap < n)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
    if (!tmp) /* allocation failure: terminate the program (optional) */
        abort();
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;             /* undefined for n<1; return 0 for safety          */

    if (n <= 6)
        return init_vals[n - 1];

    /* Initialize cache on first use */
    if (cache_len == 0) {
        ensure_capacity(6);
        for (idx_t i = 0; i < 6; ++i)
            cache[i] = init_vals[i];
        cache_len = 6;
    }

    /* Extend cache up to requested n */
    if (n > cache_len) {
        ensure_capacity(n);
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            /* Compute a(i) using the recurrence:
             * a(i) = 6*a(i-1) + 8*a(i-2) - 48*a(i-3) + 24*a(i-4) + 32*a(i-5) - 16*a(i-6)
             */
            seq_t pos = 6 * cache[i - 2] + 8 * cache[i - 3]
                      + 24 * cache[i - 5] + 32 * cache[i - 6];
            seq_t neg = 48 * cache[i - 4] + 16 * cache[i - 7];
            cache[i - 1] = pos - neg;  /* pos ≥ neg for all valid i          */
        }
        cache_len = n;
    }

    return cache[n - 1];
}
