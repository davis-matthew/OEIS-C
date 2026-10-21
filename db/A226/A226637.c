/*
 * A226637  Numbers m having with m+1 no common digit in decimal
 *          representations.
 * https://oeis.org/A226637
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 39, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   grow_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sequence A226637: the n‑th non‑negative integer m
   such that m and m+1 share no common decimal digit.  */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/grow_seq_array.h"

static seq_t *cache = NULL;
static size_t cache_cap = 0;       /* allocated capacity of cache      */
static size_t cache_len = 0;       /* number of terms stored           */
static seq_t next_candidate = 0;   /* candidate m to examine next     */

/* Return true if x and y have a common decimal digit */
static inline bool has_common_digit(seq_t x, seq_t y)
{
    uint16_t mask_x = 0, mask_y = 0;
    seq_t t = x;
    if (t == (seq_t)0)
        mask_x |= 1 << 0;
    else {
        while (t > (seq_t)0) {
            uint8_t d = (uint8_t)(t % (seq_t)10);
            mask_x |= 1 << d;
            t /= (seq_t)10;
        }
    }

    t = y;
    if (t == (seq_t)0)
        mask_y |= 1 << 0;
    else {
        while (t > (seq_t)0) {
            uint8_t d = (uint8_t)(t % (seq_t)10);
            mask_y |= 1 << d;
            t /= (seq_t)10;
        }
    }

    return (mask_x & mask_y) != 0;
}

/* Ensure the cache array has capacity for at least one more element. */
static void ensure_cache_capacity(void)
{
    if (cache_len >= cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
        grow_seq_array(&cache, &cache_cap, new_cap);
    }
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;              /* undefined for n<1, harmless */

    while (cache_len < n) {
        seq_t m = next_candidate;
        seq_t mplus = m + (seq_t)1;

        if (!has_common_digit(m, mplus)) {
            ensure_cache_capacity();
            cache[cache_len++] = m;
        }
        next_candidate = mplus;
    }

    return cache[n - 1];
}
