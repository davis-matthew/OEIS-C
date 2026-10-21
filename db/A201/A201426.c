/*
 * A201426  Number of nX2 0..2 arrays with rows and columns lexicographically
 *          nondecreasing and the instance counts of every value within one of
 *          each other.
 * https://oeis.org/A201426
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 21, u32 n <= 150, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   add_multiple
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence:
   a(n) = 7*a(n-3) - 21*a(n-6) + 35*a(n-9)
          - 35*a(n-12) + 21*a(n-15)
          - 7*a(n-18) + a(n-21)   for n > 21
   Initial terms (n = 1..21):
   3, 11, 13, 76, 160, 111, 560, 955, 539, 2447,
   3717, 1873, 7920, 11174, 5237, 21055, 28217,
   12573, 48685, 62818, 26965 */
#include <stdlib.h>
#include "../utils/add_multiple.h"

static const seq_t BASE[21] = {
    3, 11, 13, 76, 160, 111, 560, 955, 539, 2447,
    3717, 1873, 7920, 11174, 5237, 21055, 28217,
    12573, 48685, 62818, 26965
};

static seq_t *cache = NULL;          /* 1‑based indexed array of computed terms */
static idx_t  cache_max = 0;         /* largest n computed so far */
static idx_t  cache_size = 0;        /* current allocated size */

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Allocate storage on first use and initialize base terms. */
    if (cache == NULL) {
        cache_size = 21;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return 0;
        for (idx_t i = 0; i < 21; ++i) cache[i] = BASE[i];
        cache_max = 21;
    }

    /* Resize storage if needed. */
    if (n > cache_size) {
        idx_t new_size = n;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) return 0;
        cache = new_cache;
        cache_size = new_size;
    }

    /* Return already computed value. */
    if (n <= cache_max) return cache[n - 1];

    /* Compute terms sequentially up to n using the recurrence. */
    for (idx_t k = cache_max + 1; k <= n; ++k) {
        seq_t pos = 0, neg = 0;

        /* Positive part: 7*a(k-3) + 35*a(k-9) + 21*a(k-15) + 1*a(k-21) */
        add_multiple(&pos, cache[k - 3 - 1], 7);
        add_multiple(&pos, cache[k - 9 - 1], 35);
        add_multiple(&pos, cache[k - 15 - 1], 21);
        pos += cache[k - 21 - 1];

        /* Negative part: 21*a(k-6) + 35*a(k-12) + 7*a(k-18) */
        add_multiple(&neg, cache[k - 6 - 1], 21);
        add_multiple(&neg, cache[k - 12 - 1], 35);
        add_multiple(&neg, cache[k - 18 - 1], 7);

        /* Final value: positive part minus negative part (safe because pos ≥ neg). */
        cache[k - 1] = pos - neg;
    }

    cache_max = n;
    return cache[n - 1];
}
