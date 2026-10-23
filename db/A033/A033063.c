/*
 * A033063  Numbers whose base-12 representation Sum_{i=0..m} d(i)*12^i has
 *          odd d(m-i) for all odd i.
 * https://oeis.org/A033063
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 129, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   seq_is_valid
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A033063: Numbers whose base‑12 representation  Σ d(i)*12^i has odd d(m‑i) for all odd i. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/seq_is_valid.h"

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;       /* undefined for n < 1 */

    /* Return cached value if already computed */
    if (n <= cache_len)
        return cache[n - 1];

    /* Allocate/check cache */
    if (cache == NULL) {
        cache_cap = 256;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        cache_len = 0;
    }

    seq_t val;
    if (cache_len == 0)
        val = (seq_t)1;
    else
        val = cache[cache_len - 1] + 1;

    /* Generate terms until we reach n */
    while (cache_len < n) {
        if (seq_is_valid(val)) {
            if (cache_len == cache_cap) {
                cache_cap <<= 1;
                cache = (seq_t *)realloc(cache, sizeof(seq_t) * cache_cap);
            }
            cache[cache_len++] = val;
        }
        val = val + 1;
    }

    return cache[n - 1];
}
