/*
 * A054478  Numbers not divisible by any of their digits when written in base
 *          6.
 * https://oeis.org/A054478
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   numbers not divisible by any of their digits when written in
 *            base 6
 * Range:     u8 n <= 28, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   has_bad_divisor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_bad_divisor.h"

/* a(n) – nth term of A054478 */
seq_t a(idx_t n)
{
    /* cache of already computed terms (zero‑based) */
    static seq_t *cache     = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t last_checked = 1;     /* next integer to test */

    /* already have enough terms? */
    if ((idx_t)cache_len >= n)
        return cache[n - 1];

    /* initialise cache if needed */
    if (!cache) {
        cache_cap = 128;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(1);
    }

    /* extend cache until we reach n terms */
    while (cache_len < n) {
        seq_t cand = last_checked;
        while (has_bad_divisor(cand))   /* skip bad numbers */
            cand = cand + 1;

        if (cache_len >= cache_cap) {  /* grow cache */
            cache_cap <<= 1;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            if (!cache) exit(1);
        }

        cache[cache_len++] = cand;
        last_checked = cand + 1;
    }

    return cache[n - 1];
}
