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
 * Validated: standard, pass, 25 terms through n = 25 (2026-11-03)
 * Helpers:   ipow, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow.h"
#include "../utils/mem.h"

static seq_t *cache = NULL;
static size_t cache_size = 0;

// ensure cache is large enough and initialized to 0
static void ensure_cache(idx_t n) {
    if (cache == NULL || cache_size <= n) {
        size_t old = cache_size;
        cache_size = n + 1;
        cache = mem_resize(seq_t, cache, cache_size);
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
    seq_t *binoms = mem_new(seq_t, (n + 1));
    binoms[0] = 1;                    // C(n,0)=1
    for (idx_t j = 1; j <= n; ++j) {
        // binoms[j] = binoms[j-1] * (n-j+1) / j
        seq_t num = binoms[j - 1] * (seq_t)(n - j + 1);
        binoms[j] = num / (seq_t)j;   // exact division
    }

    seq_t sum = 0;
    for (idx_t j = 0; j <= n; ++j) {
        seq_t p = ipow(binoms[j], e);
        sum += p;
    }
    seq_t result = sum / 2;          // division is exact

    cache[n] = result;
    mem_free(binoms);
    return result;
}
