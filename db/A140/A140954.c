/*
 * A140954  Half the number of n X n binary matrices, symmetric under vertical
 *          reflection, having equal row sums.
 * https://oeis.org/A140954
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1/2 * Σ_{s=0}^{n} C(n,s)^{ceil(n/2)}
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/big_pow.h"

static seq_t *cache = NULL;
static size_t cache_size = 0;

// ensure cache is large enough and initialized to 0
static void ensure_cache(idx_t n) {
    if (cache == NULL || cache_size <= n) {
        size_t old = cache_size;
        cache_size = n + 1;
        cache = (seq_t *)realloc(cache, cache_size * sizeof(seq_t));
        for (size_t i = old; i < cache_size; ++i)
            cache[i] = 0;          // 0 is convertible to any seq_t
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;     // undefined for offset 1, but harmless

    ensure_cache(n);
    if (cache[n] != 0)                // already computed
        return cache[n];

    idx_t e = (n + 1) / 2;            // ceil(n/2)

    /* binomial coefficients binoms[s] = C(n,s) */
    seq_t *binoms = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    binoms[0] = 1;                    // C(n,0)=1
    for (idx_t j = 1; j <= n; ++j) {
        // binoms[j] = binoms[j-1] * (n-j+1) / j
        seq_t num = binoms[j - 1] * (seq_t)(n - j + 1);
        binoms[j] = num / (seq_t)j;   // exact division
    }

    seq_t sum = 0;
    for (idx_t j = 0; j <= n; ++j) {
        seq_t p = big_pow(binoms[j], e);
        sum += p;
    }
    seq_t result = sum / 2;          // division is exact

    cache[n] = result;
    free(binoms);
    return result;
}
