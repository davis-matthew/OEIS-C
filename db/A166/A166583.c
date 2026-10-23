/*
 * A166583  Number of reduced words of length n in Coxeter group on 15
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^12 = I.
 * https://oeis.org/A166583
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0..11) already defined. a(12) = 13 * Σ_{j=1}^{11} a(12‑j) − 90
 *            a(n) = 13 * Σ_{j=1}^{11} a(n‑j) − 91 * a(n‑12) (n ≥ 13)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;

static void ensure_cache(idx_t n)
{
    /* Ensure cache has capacity up to index n (inclusive). */
    idx_t needed = n + 1;
    if (cache == NULL) {
        cache = (seq_t*)calloc(needed, sizeof(seq_t));
        if (cache == NULL) exit(1);
    } else if (needed > cache_len) {
        seq_t *tmp = (seq_t*)realloc(cache, needed * sizeof(seq_t));
        if (tmp == NULL) exit(1);
        cache = tmp;
    }
    cache_len = needed;

    /* Initialise the known first 12 terms. */
    static const seq_t init_vals[12] = {
        1ULL, 15ULL, 210ULL, 2940ULL, 41160ULL,
        576240ULL, 8067360ULL, 112943040ULL,
        1581202560ULL, 22136835840ULL,
        309915701760ULL, 4338819824640ULL
    };
    for (idx_t i = 0; i < 12 && i < cache_len; ++i)
        cache[i] = init_vals[i];

    /* Compute missing terms. */
    for (idx_t i = 12; i < cache_len; ++i) {
        seq_t sum = 0;
        for (idx_t j = 1; j <= 11; ++j)
            sum += cache[i - j];
        if (i == 12)
            cache[i] = 13 * sum - (seq_t)90;
        else
            cache[i] = 13 * sum - 91 * cache[i - 12];
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
