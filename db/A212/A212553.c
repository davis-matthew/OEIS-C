/*
 * A212553  Irreducible near matchings over 2n+1 vertices.
 * https://oeis.org/A212553
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000699(n+1)*(2*n+1)*(n+1)/(2*n), where A000699(n) =
 *            (n-1)*Sum_{i=1..n-1} A000699(i)*A000699(n-i), A000699(0) =
 *            A000699(1) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 17,
 *            big any n
 * Validated: partial, pass, 17 terms through n = 17 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

static seq_t A000699_a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t cache_len = 0;

    if (n == 0 || n == 1) return 1;

    if (cache == NULL) {
        cache_cap = 16;
        cache = mem_new(seq_t, cache_cap);
        cache[0] = 1;
        cache[1] = 1;
        cache_len = 2;
    }

    while (cache_len <= n) {
        if (cache_len >= cache_cap) {
            cache_cap *= 2;
            seq_t *new_cache = mem_new(seq_t, cache_cap);
            mem_copy(new_cache, cache, cache_len * sizeof(seq_t));
            mem_free(cache);
            cache = new_cache;
        }

        idx_t k = cache_len;
        seq_t sum = 0;
        for (idx_t i = 1; i < k; i++) {
            sum = sum + cache[i] * cache[k - i];
        }
        cache[k] = ((seq_t)(k - 1)) * sum;
        cache_len++;
    }

    return cache[n];
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    /* A000699(n+1) = n * S, with S = Sum_{i=1..n} A000699(i)*A000699(n+1-i) */
    A000699_a(n + 1);
    seq_t S = 0;
    for (idx_t i = 1; i <= n; i++) {
        S = S + A000699_a(i) * A000699_a(n + 1 - i);
    }
    seq_t t = (seq_t)n + 1;
    seq_t u = (seq_t)2 * (seq_t)n + 1;
    if ((t % 2) == 0) {
        t = t / 2;
    } else {
        S = S / 2;
    }
    return S * t * u;
}
