/*
 * A208552  Number of n X 5 0..1 arrays avoiding 0 0 0 and 0 0 1 horizontally
 *          and 0 0 1 and 0 1 1 vertically.
 * https://oeis.org/A208552
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the linear recurrence a(n) = 2*a(n-1) + 4*a(n-2)
 *            - 10*a(n-3) - 5*a(n-4) + 20*a(n-5) - 20*a(n-7) + 5*a(n-8) +
 *            10*a(n-9) - 4*a(n-10) - 2*a(n-11) + a(n-12) for n ≥ 13. Initial
 *            values: a(1) = 16, a(2) = 256, a(3) = 1008, a(4) = 3969, a(5) =
 *            10080, a(6) = 25600, a(7) = 52000, a(8) = 105...
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 54, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>   /* for size_t */

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_size = 0;

/* Ensure that cache has capacity for at least indices up to n (1‑based). */
static void ensure_cache(idx_t n)
{
    if (n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap <= (size_t)n + 1)
            new_cap *= 2;
        seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        /* realloc() may return NULL only if new_cap == 0, which cannot happen here. */
        cache = new_cache;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined for n<1, return 0 for safety */

    ensure_cache(n);

    /* Compute terms up to n if not already done. */
    for (idx_t k = cache_size + 1; k <= n; ++k) {
        if (k <= 12) {
            /* Initial values (index 1..12). */
            static const seq_t init_vals[13] = {
                0,          /* unused 0th index */
                16,       /* a(1) */
                256,      /* a(2) */
                1008,     /* a(3) */
                3969,     /* a(4) */
                10080,    /* a(5) */
                25600,    /* a(6) */
                52000,    /* a(7) */
                105625,   /* a(8) */
                187200,   /* a(9) */
                331776,   /* a(10) */
                536256,   /* a(11) */
                866761    /* a(12) */
            };
            cache[k] = init_vals[k];
        } else {
            /* Compute using the recurrence.  All operations stay unsigned
               because the recurrence guarantees the result is non‑negative. */
            seq_t sum1 = 2 * cache[k-1]
                       + 4 * cache[k-2]
                       + 20 * cache[k-5]
                       + 5 * cache[k-8]
                       + 10 * cache[k-9]
                       + 1 * cache[k-12];

            seq_t sum2 = 10 * cache[k-3]
                       + 5 * cache[k-4]
                       + 20 * cache[k-7]
                       + 4 * cache[k-10]
                       + 2 * cache[k-11];

            cache[k] = sum1 - sum2;
        }
    }
    cache_size = n;
    return cache[n];
}
