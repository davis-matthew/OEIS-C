/*
 * A170176  Number of reduced words of length n in Coxeter group on 23
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^39 = I.
 * https://oeis.org/A170176
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1. For 1 ≤ n ≤ 38: a(n) = 2 + 21 * Σ_{i=0}^{n-1} a(i).
 *            For n = 39: a(39) = 1 + 21 * Σ_{i=1}^{38} a(i) - 231 * a(0). For
 *            n ≥ 40: a(n) = 21 * Σ_{i=1}^{38} a(n-i) - 231 * a(n-39)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   w_term
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/w_term.h"

/* External integer types as defined by the build. */
extern seq_t a(idx_t n);

/* The memoised sequence array. */
static seq_t *cache      = NULL;
static size_t cache_size = 0;
static idx_t last_n      = (idx_t)-1;   /* indicates no value yet */

seq_t a(idx_t n)
{
    /* Allocate or grow the cache if necessary. */
    size_t target = (size_t)(n + 1);
    if (target > cache_size) {
        seq_t *new_cache = (seq_t *)realloc(cache, target * sizeof(seq_t));
        if (!new_cache) { /* allocation failure – abort */
            abort();
        }
        /* Initialise new entries to zero (not strictly needed). */
        for (size_t i = cache_size; i < target; ++i) new_cache[i] = (seq_t)0;
        cache      = new_cache;
        cache_size = target;
    }

    /* Compute terms from last_n+1 up to n. */
    for (idx_t k = (last_n == (idx_t)-1) ? 0 : last_n + 1; k <= n; ++k) {
        size_t idx = (size_t)k;
        if (k == 0) {
            cache[idx] = (seq_t)1;
            continue;
        }

        if (k <= 38) {
            seq_t sum = 0;
            for (idx_t i = 0; i < k; ++i) sum += cache[(size_t)i];
            cache[idx] = w_term(k) + (seq_t)21 * sum;
        } else if (k == 39) {
            seq_t sum = 0;
            for (idx_t i = 1; i <= 38; ++i) sum += cache[(size_t)(39 - i)];
            cache[idx] = w_term(39) + (seq_t)21 * sum - (seq_t)231 * cache[0];
        } else {
            /* k ≥ 40 */
            seq_t sum = 0;
            for (idx_t i = 1; i <= 38; ++i) sum += cache[(size_t)(k - i)];
            cache[idx] = (seq_t)21 * sum - (seq_t)231 * cache[(size_t)(k - 39)];
        }
    }

    last_n = n;
    return cache[(size_t)n];
}
