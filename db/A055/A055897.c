/*
 * A055897  a(n) = n*(n-1)^(n-1).
 * https://oeis.org/A055897
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(n-1)^(n-1) (with 0^0 treated as 1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 194 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

/* Make sure the cache is large enough */
static void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr) {
    seq_t *cache = *cache_ptr;
    size_t cap = *cap_ptr;
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? (cap << 1) : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp && new_cap > 0) {
            /* Allocation failed – abort the program. */
            exit(1);
        }
        if (tmp) {
            /* Initialize newly allocated slots to zero. */
            if (!cache) {
                for (size_t i = 0; i < new_cap; ++i)
                    tmp[i] = 0;
            } else {
                for (size_t i = cap; i < new_cap; ++i)
                    tmp[i] = 0;
            }
            *cache_ptr = tmp;
            *cap_ptr = new_cap;
        }
    }
}

/* Exponentiation by squaring for seq_t */
static seq_t pow_seq_t(seq_t base, idx_t exp) {
    seq_t result = 1;
    while (exp) {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        if (exp)
            base *= base;
    }
    return result;
}

/* Main sequence function */
seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;

    if (n == 0)
        return 0;                 /* undefined for n=0 – return 0 */

    ensure_cache(n, &cache, &cache_cap);

    /* Return cached value if already computed */
    if (cache[n] != 0)
        return cache[n];

    seq_t base  = (seq_t)(n - 1);
    seq_t power = pow_seq_t(base, n - 1);
    seq_t result = (seq_t)n * power;

    cache[n] = result;             /* cache the computed term */
    return result;
}
