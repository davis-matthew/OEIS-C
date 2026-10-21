/*
 * A057315  Coordination sequence T2 for Zeolite Code SAV.
 * https://oeis.org/A057315
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 192, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A057315: Coordination sequence T2 for Zeolite Code SAV.
   The sequence satisfies the linear recurrence
         a(n) = -∑_{k=1}^{17} d_k a(n-k),
   for all n ≥ 18, where
         d = [-2, 2, -3, 4, -4, 4, -4, 4, -4, 4, -4, 4, -4, 3, -2, 2, -1].
   The first 18 terms are supplied explicitly: a(0) … a(17). */
#include <stdlib.h>

static const seq_t init[18] = {
    (seq_t)1,  (seq_t)4,  (seq_t)9,  (seq_t)17, (seq_t)29, (seq_t)45,
    (seq_t)65, (seq_t)88, (seq_t)114, (seq_t)144, (seq_t)177,
    (seq_t)214, (seq_t)256, (seq_t)300, (seq_t)347, (seq_t)399,
    (seq_t)453, (seq_t)510
};
static const int coeff[17] = {
    -2, 2, -3, 4, -4, 4, -4, 4, -4, 4, -4, 4, -4, 3, -2, 2, -1
};

static seq_t *cache = NULL;   /* Cache all computed terms a(0) … a(n). */
static idx_t cache_len = 0;   /* Number of terms stored in cache. */

seq_t a(idx_t n)
{
    /* Return the first 18 terms directly. */
    if (n < 18) return init[(size_t)n];

    /* Initialize the cache on first use. */
    if (cache == NULL) {
        cache_len = 18;
        cache = (seq_t *)malloc((size_t)cache_len * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
        for (idx_t i = 0; i < 18; ++i) cache[i] = init[i];
    }

    /* Grow the cache if needed and compute missing terms. */
    if (n >= cache_len) {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t sum = 0;
            for (int k = 0; k < 17; ++k) {
                sum += (seq_t)coeff[k] * cache[i - 1 - k];
            }
            cache[i] = -sum;  /* unsigned modulo two's complement gives the correct result. */
        }
        cache_len = (size_t)n + 1;
    }

    return cache[n];
}
