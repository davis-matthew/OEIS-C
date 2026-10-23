/*
 * A039393  Numbers whose base-10 representation has the same nonzero number
 *          of 0's and 1's.
 * https://oeis.org/A039393
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number with the same nonzero number of 0's and 1's in its
 *            decimal representation
 * Range:     u8 n <= 19, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   digit_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_counts.h"

/* static helpers and cache */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t last_candidate = 9;   /* last tested number (just before first term) */

/* check whether a number has the same nonzero number of 0's and 1's */
static bool equal_zero_one(seq_t x)
{
    unsigned char cnt[10] = {0};
    digit_counts(x, cnt);
    return cnt[0] == cnt[1] && cnt[0] > 0;
}

seq_t a(idx_t n)
{
    /* assure the cache holds at least n terms */
    while (cache_len < (size_t)n) {
        /* advance to next candidate */
        last_candidate++;
        seq_t cand = (seq_t)last_candidate;
        if (equal_zero_one(cand)) {
            /* enlarge cache if needed */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap == 0 ? 32 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();            /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = cand;
        }
    }
    return cache[(size_t)n - 1];
}
