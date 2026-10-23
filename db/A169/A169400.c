/*
 * A169400  Number of reduced words of length n in Coxeter group on 3
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^32 = I.
 * https://oeis.org/A169400
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence from the generating function:
   a(0) = 1;
   for 1 ≤ n ≤ 31:  a(n) = Σ_{k=0}^{n-1} a(k) + 2;
   for n ≥ 32: a(n) = Σ_{k=n-31}^{n-1} a(k) - a(n-32).   */
/* The sequence uses the same recurrence as A003945 for n < 32, then diverges at n = 32. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static size_t cache_cap = 0;

    /* Inicialise cache with first 33 terms (0 … 32). */
    if (!cache) {
        cache_cap = 33;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        cache_len = 0;
        for (idx_t i = 0; i < 33; ++i) {
            seq_t sum = 0;
            idx_t start = (i > 31) ? (i - 31) : 0;
            for (idx_t k = start; k < i; ++k) sum += cache[(size_t)k];
            if (i >= 32) sum -= cache[(size_t)(i - 32)];
            seq_t pi = 1;
            if (i >= 1 && i <= 31) pi = 2;
            if (i > 32) pi = 0;
            cache[(size_t)i] = sum + pi;
        }
        cache_len = 33;
    }

    /* Grow the cache if necessary. */
    if (n + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)(n + 1)) new_cap <<= 1;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n. */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t sum = 0;
        idx_t start = (i > 31) ? (i - 31) : 0;
        for (idx_t k = start; k < i; ++k) sum += cache[(size_t)k];
        if (i >= 32) sum -= cache[(size_t)(i - 32)];
        seq_t pi = 1;
        if (i >= 1 && i <= 31) pi = 2;
        if (i > 32) pi = 0;
        cache[(size_t)i] = sum + pi;
    }
    cache_len = n + 1;

    return cache[(size_t)n];
}
