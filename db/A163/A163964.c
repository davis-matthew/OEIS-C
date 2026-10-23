/*
 * A163964  Number of reduced words of length n in Coxeter group on 16
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^6 = I.
 * https://oeis.org/A163964
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-3) + a(n-4) + a(n-5) - 105*a(n-6) +
 *            1 for n = 6 * a(n) = 14*(a(n-1)+a(n-2)+a(n-3)+a(n-4)+a(n-5)) -
 *            105*a(n-6) for n > 6 * Initial values (n = 0..5): 1, 16, 240,
 *            3600, 54000, 810000 * * This implementation memoises computed
 *            terms in a dynamic array so that any call * o...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_vals[6] = {1, 16, 240, 3600, 54000, 810000};
static seq_t *cache = NULL;          /* memoised sequence values */
static size_t cache_len = 0;         /* number of values currently computed (next free index) */
static size_t cache_cap = 0;         /* allocated capacity of cache */

/* compute a(n) */
seq_t a(idx_t n)
{
    /* directly handle the first six terms */
    if ((size_t)n < 6)
        return init_vals[(size_t)n];

    /* initialise cache if this is the first call */
    if (cache == NULL) {
        cache_cap = 32;
        if (cache_cap <= (size_t)n + 1)
            cache_cap = (size_t)n + 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            return 0; /* allocation failed – unlikely in tests */

        /* copy the first six known values */
        for (size_t i = 0; i < 6; ++i)
            cache[i] = init_vals[i];

        /* compute a(6) with the special +1 term */
        seq_t sum6 = cache[5] + cache[4] + cache[3] + cache[2] + cache[1];
        seq_t a6 = (seq_t)14 * sum6 - (seq_t)105 * cache[0] + (seq_t)1;
        cache[6] = a6;

        cache_len = 7;          /* indices 0..6 have been computed */
    }

    /* ensure capacity for all terms up to n */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap <= (size_t)n + 1)
            new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            return 0; /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute any missing terms in order */
    if ((size_t)n >= cache_len) {
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            seq_t sum5 = cache[i-1] + cache[i-2] + cache[i-3] + cache[i-4] + cache[i-5];
            seq_t val   = (seq_t)14 * sum5 - (seq_t)105 * cache[i-6];
            cache[i] = val;
        }
        cache_len = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
