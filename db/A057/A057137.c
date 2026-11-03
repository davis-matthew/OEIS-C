/*
 * A057137  Concatenate next digit at right hand end (where the next digit
 *          after 9 is again 0).
 * https://oeis.org/A057137
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = 10 * a(n-1) + (n mod 10), a(0) = 0
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 196 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* static cache to keep values already computed across calls */
    static seq_t *cache = NULL;   /* array of a(i) for i < cache_len */
    static idx_t cache_len = 0;   /* number of valid entries in cache */

    /* initialise cache with a(0) */
    if (cache_len == 0) {
        cache_len = 1;
        cache     = mem_new(seq_t, 1);
        if (cache == NULL) return (seq_t)0;  /* out‑of‑memory fallback */
        cache[0] = (seq_t)0;
    }

    /* extend cache if necessary */
    if (n >= cache_len) {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_len);
        if (tmp == NULL) return cache[cache_len - 1]; /* fallback */
        cache = tmp;
        /* compute missing terms */
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t digit = (seq_t)(i % 10);      /* digit appended to the right */
            cache[i] = cache[i - 1] * 10 + digit;
        }
        cache_len = new_len;
    }

    return cache[n];
}
