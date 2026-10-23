/*
 * A023732  Numbers with exactly 3 2's in base 5 expansion.
 * https://oeis.org/A023732
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   count_twos_base5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/count_twos_base5.h"

/* Computation: successive integers whose base‑5 representation
 * contains exactly three digits equal to 2. */

static seq_t *cache = NULL;          /* cached terms */
static size_t cache_len = 0;         /* number of cached terms */
static size_t cache_cap = 0;         /* capacity of cache */
static seq_t last_val = 0;           /* last term stored */

seq_t a(idx_t n)
{
    /* initialise cache on first call */
    if (cache_len == 0) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        last_val = 0;
    }

    /* generate further terms until the requested index is reached */
    while (cache_len < n) {
        seq_t x = last_val + 1;          /* start searching after last term */

        /* find next number with exactly three 2's in base‑5 */
        while (1) {
            if (count_twos_base5(x) == 3)
                break;
            ++x;
        }

        /* grow cache if needed */
        if (cache_len == cache_cap) {
            cache_cap <<= 1;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }

        cache[cache_len++] = x;          /* store the new term */
        last_val = x;
    }

    return cache[n - 1];
}
