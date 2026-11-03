/*
 * A198857  a(n) = 10*8^n - 1.
 * https://oeis.org/A198857
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 9, a(n) = 8*a(n-1) + 7 for n > 0
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 928 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* store computed terms */
    static idx_t cache_len = 0;      /* number of valid elements */
    static idx_t cache_cap = 0;      /* allocated capacity */

    /* first call: allocate cache + initialize a(0) */
    if (cache == NULL) {
        cache_cap = 1;
        cache = mem_new(seq_t, cache_cap);
        if (cache == NULL) return (seq_t)0; /* allocation failure */
        cache[0] = (seq_t)9;              /* a(0) */
        cache_len = 1;
    }

    /* grow cache if needed */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (tmp == NULL) return (seq_t)0;  /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    if (n >= cache_len) {
        for (idx_t i = cache_len; i <= n; ++i) {
            cache[i] = cache[i-1] * (seq_t)8 + (seq_t)7;
        }
        cache_len = n + 1;
    }

    return cache[n];
}
