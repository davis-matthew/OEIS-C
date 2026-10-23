/*
 * A116972  a(n) = 11*3^n - 2*n - 10.
 * https://oeis.org/A116972
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11*3^n - 2*n - 10 Recurrence: a(0) = 1; a(n) = 3*a(n-1) +
 *            4*n + 14 (n >= 1)
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 18, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t* cache = NULL;
static size_t cache_cap = 0;

seq_t a(idx_t n) {
    if (cache == NULL) {
        cache = (seq_t*)malloc(sizeof(seq_t));
        if (!cache) return (seq_t)0;                /* memory failure */
        cache[0] = 1;                              /* a(0) = 1 */
        cache_cap = 1;
    }

    if ((idx_t)cache_cap <= n) {
        size_t new_cap = (size_t)n + 1;
        seq_t* new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;           /* memory failure */
        cache = new_cache;
        /* Compute terms from cache_cap to n using the recurrence */
        for (idx_t i = (idx_t)cache_cap; i <= n; ++i) {
            seq_t val = cache[i - 1] * 3;
            val += (seq_t)(4 * i + 14);
            cache[i] = val;
        }
        cache_cap = new_cap;
    }
    return cache[n];
}
