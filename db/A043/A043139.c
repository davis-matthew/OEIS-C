/*
 * A043139  Numbers k such that 1 and 4 occur juxtaposed in the base-7
 *          representation of k but not of k-1.
 * https://oeis.org/A043139
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   has14
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/has14.h"

static seq_t *cache = NULL;
static size_t   cache_len = 0;
static size_t   cache_cap = 0;
static seq_t    next_k    = 1;

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* offset starts at 1 */

    if ((idx_t)cache_len >= n)
        return cache[n-1];

    while ((idx_t)cache_len < n) {
        if (has14(next_k) && !has14(next_k - 1)) {
            if (cache_len >= cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 32;
                seq_t *np = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!np) abort();
                cache = np;
                cache_cap = new_cap;
            }
            cache[cache_len++] = next_k;
        }
        ++next_k;
    }

    return cache[n-1];
}
