/*
 * A169028  Number of reduced words of length n in Coxeter group on 15
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^24 = I.
 * https://oeis.org/A169028
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(1) = 15 a(n) = 14·a(n-1) for 2 ≤ n < 24 a(n) =
 *            13·(a(n-1)+…+a(n-23)) – 91·a(n-24) for n ≥ 24 (the recurrence
 *            comes from the rational generating function given in the OEIS
 *            entry)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;
static size_t cache_size = 0;
static idx_t computed = 0;

/* Ensure that cache[0..n] is computed and cached. */
static void ensure_cache(idx_t n)
{
    /* Grow the cache if necessary. */
    if (n + 1 > cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (!new_cache) exit(1);
        if (cache) {
            memcpy(new_cache, cache, computed * sizeof(seq_t));
            free(cache);
        }
        cache     = new_cache;
        cache_size = new_size;
    }

    /* Compute missing terms. */
    for (idx_t k = computed; k <= n; ++k) {
        if (k == 0) {
            cache[0] = (seq_t)1;
        } else if (k == 1) {
            cache[1] = (seq_t)15;
        } else if (k < 24) {
            cache[k] = (seq_t)14 * cache[k-1];
        } else {
            /* recurrence: a(k) = 13*Σ_{i=1}^{23} a(k-i) – 91*a(k-24) */
            seq_t sum = (seq_t)0;
            for (idx_t i = 1; i <= 23; ++i) {
                sum += cache[k - i];
            }
            seq_t val = sum * (seq_t)13;
            val -= cache[k - 24] * (seq_t)91;
            cache[k] = val;
        }
    }
    computed = (size_t)n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
