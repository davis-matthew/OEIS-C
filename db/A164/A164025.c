/*
 * A164025  Number of reduced words of length n in Coxeter group on 28
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^6 = I.
 * https://oeis.org/A164025
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 26*(a(n-1)+a(n-2)+a(n-3)+a(n-4)+a(n-5)) - 351*a(n-6) for
 *            n >= 7 Initial values (offset 0): a(0) = 1 a(1) = 28 a(2) = 756
 *            a(3) = 20412 a(4) = 551124 a(5) = 14880348 a(6) = 401769018
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_values[7] = {
    (seq_t)1,
    (seq_t)28,
    (seq_t)756,
    (seq_t)20412,
    (seq_t)551124,
    (seq_t)14880348,
    (seq_t)401769018
};

static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of terms stored (highest index + 1) */
static size_t cache_cap = 0;   /* allocated capacity */

static void ensure_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    if (new_cap < needed)
        new_cap = needed;
    seq_t *ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = ptr;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Map idx_t to usize for array indices safely */
    if (n < 7)
        return init_values[(size_t)n];

    /* Allocate or enlarge cache if necessary */
    if (cache_len == 0) {
        /* First call: allocate at least up to the requested n or 7 */
        size_t init_cap = (size_t)n + 1;
        if (init_cap < 8)
            init_cap = 8;
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        cache_cap = init_cap;

        /* Copy initial values */
        for (size_t i = 0; i < 7; ++i)
            cache[i] = init_values[i];
        cache_len = 7;
    }

    if ((size_t)n >= cache_len) {
        ensure_capacity((size_t)n + 1);
        /* Compute missing terms up to n */
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            seq_t sum = 0;
            for (size_t k = 1; k <= 5; ++k)
                sum += cache[i - k];
            cache[i] = ((seq_t)26 * sum) - ((seq_t)351 * cache[i - 6]);
        }
        cache_len = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
