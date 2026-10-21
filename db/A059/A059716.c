/*
 * A059716  Number of column convex polyominoes with n hexagonal cells.
 * https://oeis.org/A059716
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 10*a(n-2) + 7*a(n-3) - a(n-4) (n >= 5); * a(1)
 *            = 1, a(2) = 3, a(3) = 11, a(4) = 42
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 34, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;          /* undefined, but safe */

    /* ensure we have a minimum of 4 terms pre‑computed */
    size_t target = (n > 4 ? n : 4);

    /* grow the storage if necessary */
    if (cache_cap < target) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap < target) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) { /* allocation failed – return 0 as fallback */
            return (seq_t)0;
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise first four values on first use */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)11;
        cache[3] = (seq_t)42;
        cache_len = 4;
    }

    /* compute missing terms up to n */
    if (cache_len < n) {
        for (size_t idx = cache_len; idx < n; ++idx) {
            /* recurrence: a[i] = 6*a[i-1] - 10*a[i-2] + 7*a[i-3] - a[i-4]  */
            seq_t pos = 6 * cache[idx - 1] + 7 * cache[idx - 3];
            seq_t neg = 10 * cache[idx - 2] +       cache[idx - 4];
            cache[idx] = pos - neg;
        }
        cache_len = n;
    }

    return cache[n - 1];
}
