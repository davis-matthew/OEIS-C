/*
 * A195946  Powers of 11 which have no zero in their decimal expansion.
 * https://oeis.org/A195946
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   big_pow, digit_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: generate powers of 11 whose decimal expansion has no zero digit.
   a(n) is the nth such power, where n starts at 1. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/big_pow.h"
#include "../utils/digit_counts.h"

static seq_t *cache = NULL;     /* array of computed terms */
static size_t cache_len = 0;    /* number of stored terms */
static size_t cache_cap = 0;    /* allocated capacity */
static idx_t next_exp = 0;      /* exponent to test next */

seq_t a(idx_t n)
{
    /* Ensure we have at least n terms in the cache */
    while (cache_len < (size_t)n) {
        /* Grow the cache if necessary */
        if (cache_len >= cache_cap) {
            size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache_cap = new_cap;
        }

        /* Compute 11^next_exp */
        seq_t pow = big_pow((seq_t)11, (seq_t)next_exp);

        /* Check for zero digits */
        unsigned char cnt[10];
        digit_counts(pow, cnt);
        if (cnt[0] == 0) {
            cache[cache_len++] = pow;   /* store the term */
        }

        next_exp++;   /* move to the next exponent */
    }

    return cache[n - 1]; /* 1‑based indexing */
}
