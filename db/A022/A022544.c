/*
 * A022544  Numbers that are not the sum of 2 squares.
 * https://oeis.org/A022544
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   this function returns the n‑th positive integer that is NOT the
 *            sum of two squares. The definition used is: an integer is NOT a
 *            sum of two squares iff it has a prime factor p ≡ 3 (mod 4)
 *            raised to an odd power. This implementation incrementally builds
 *            the sequence in a static cache and performs t...
 * Range:     u8 n <= 159, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_not_sum_two_squares
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_not_sum_two_squares.h"

/* static cache to store computed terms */
static seq_t *cache = NULL;      /* array of computed values */
static size_t cache_cap = 0;     /* current allocated capacity */
static size_t cache_len = 0;     /* number of terms stored */

/* next integer to test for being NOT a sum of two squares */
static idx_t next_candidate = 3;

/* grow the cache to at least 'need' elements */
static void grow_cache(size_t need)
{
    if (cache_cap < need) {
        size_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* fill the cache with terms up to at least position 'n' (1‑based) */
static void ensure_cache(idx_t n)
{
    while (cache_len < n) {
        /* search for the next integer that is NOT a sum of two squares */
        while (true) {
            if (is_not_sum_two_squares((seq_t)next_candidate)) {
                grow_cache(cache_len + 1);
                cache[cache_len++] = (seq_t)next_candidate;
                next_candidate++;
                break;
            }
            next_candidate++;
        }
    }
}

/* public function required by the build */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined below offset, just return 0 */
    ensure_cache(n);
    return cache[n - 1];          /* 0‑based indexing into the cached array */
}
