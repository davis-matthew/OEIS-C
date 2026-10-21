/*
 * A220710  Number of ways to reciprocally link elements of an 5 X n array
 *          either to themselves or to exactly two horizontal and antidiagonal
 *          neighbors, without consecutive collinear links.
 * https://oeis.org/A220710
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 7*a(n-2) - 14*a(n-3) - 11*a(n-4) + 17*a(n-5) +
 *            5*a(n-6) - 6*a(n-7) for n > 11 * Initial values: * a(1) = 1,
 *            a(2) = 1, a(3) = 8, a(4) = 28, a(5) = 107, a(6) = 405, * a(7) =
 *            1520, a(8) = 5706, a(9) = 21418, a(10) = 80390, a(11) = 301736
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 35, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Static cache for already computed terms (index 0 unused) */
static seq_t *cache = NULL;
static idx_t cache_size = 0;

/* Initial sequence values for n = 1 .. 11 */
static const seq_t init_vals[] = {
    0ULL, /* placeholder for index 0 */
    1ULL, 1ULL, 8ULL, 28ULL, 107ULL, 405ULL,
    1520ULL, 5706ULL, 21418ULL, 80390ULL, 301736ULL
};

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;          /* undefined for n < 1 */

    /* Initialise cache on first call */
    if (!cache) {
        cache_size = sizeof(init_vals) / sizeof(init_vals[0]);   /* includes index 0 */
        cache = (seq_t *)calloc(cache_size, sizeof(seq_t));
        if (!cache) abort();                                 /* memory error */

        for (idx_t i = 1; i < cache_size; ++i)
            cache[i] = init_vals[i];
    }

    /* Return cached value if available */
    if (n < cache_size && cache[n] != 0)
        return cache[n];

    /* Extend cache to accommodate index n */
    if (n >= cache_size) {
        idx_t old_size = cache_size;
        idx_t new_size = n + 1;                 /* needed indices: 0 .. n */
        seq_t *new_cache = (seq_t *)calloc(new_size, sizeof(seq_t));
        if (!new_cache) abort();

        /* copy old values */
        for (idx_t i = 0; i < old_size; ++i)
            new_cache[i] = cache[i];

        free(cache);
        cache = new_cache;
        cache_size = new_size;

        /* compute terms from old_size up to n */
        for (idx_t k = old_size; k <= n; ++k) {
            if (k <= 11) {                     /* should have been initialised */
                cache[k] = init_vals[k];
                continue;
            }
            /* compute using the recurrence */
            seq_t sum = 3 * cache[k - 1] + 7 * cache[k - 2]
                      + 17 * cache[k - 5] + 5 * cache[k - 6];
            seq_t sub = 14 * cache[k - 3] + 11 * cache[k - 4]
                      + 6 * cache[k - 7];
            cache[k] = sum - sub;                /* unsigned arithmetic; sub <= sum */
        }
    } else {
        /* compute missing terms between current cache_size and n */
        for (idx_t k = cache_size; k <= n; ++k) {
            seq_t sum = 3 * cache[k - 1] + 7 * cache[k - 2]
                      + 17 * cache[k - 5] + 5 * cache[k - 6];
            seq_t sub = 14 * cache[k - 3] + 11 * cache[k - 4]
                      + 6 * cache[k - 7];
            cache[k] = sum - sub;
        }
        cache_size = n + 1;
    }

    return cache[n];
}
