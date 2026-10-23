/*
 * A015980  Four iterations of Reverse and Add are needed to reach a
 *          palindrome.
 * https://oeis.org/A015980
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 42, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_palindrome, reverse_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) = the n‑th integer that needs exactly 4 reverse-and‑add
   iterations to reach a palindrome.  The implementation scans positive
   integers, runs the reverse‑and‑add process up to four times, and,
   whenever the palindrome appears on the fourth iteration, stores the
   starting number in a cache for fast subsequent look‑ups. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/reverse_decimal.h"
#include "../utils/is_palindrome.h"

static seq_t *cache = NULL;          /* cache of found terms          */
static size_t cache_len = 0;         /* number of cached terms        */
static size_t cache_cap = 0;         /* allocated size of cache       */
static seq_t next_start = 0;         /* next number to test           */

static void add_to_cache(seq_t val) {
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = new_cache;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

seq_t a(idx_t n) {
    while (cache_len <= (size_t)n) {
        seq_t orig = next_start;
        seq_t x = orig;
        bool hit = false;

        for (int k = 1; k <= 4; ++k) {
            seq_t rev = reverse_decimal(x);
            x = x + rev;
            if (is_palindrome(x, 10)) {
                if (k == 4) hit = true;
                break;
            }
        }
        if (hit) add_to_cache(orig);
        next_start++;          /* move to next candidate         */
    }
    return cache[(size_t)n];
}
