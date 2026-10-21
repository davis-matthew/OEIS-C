/*
 * A206286  Nonprime numbers starting with a digit 1.
 * https://oeis.org/A206286
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   is_prime, leading_digit_is_one, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the n‑th composite number (including 1) whose
   decimal representation begins with the digit 1. */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/seq_realloc.h"
#include "../utils/leading_digit_is_one.h"

/* Cache of already generated terms. */
static seq_t *cache = NULL;
static size_t cached_len = 0;
static size_t cached_cap = 0;

/* Last integer examined in the search (starts at 1). */
static idx_t next_candidate = 1;

/* Compute the nth term (a(1) is the first term). */
seq_t a(idx_t n)
{
    /* Grow cache as needed. */
    while (cached_len < (size_t)n) {

        /* Search for the next qualifying number. */
        while (true) {
            if (leading_digit_is_one(next_candidate) && !is_prime(next_candidate)) {
                /* Ensure cache capacity. */
                if (cached_len == cached_cap) {
                    size_t new_cap = (cached_cap == 0) ? 16 : cached_cap << 1;
                    cache = seq_realloc(cache, cached_cap, new_cap);
                    cached_cap = new_cap;
                }
                cache[cached_len] = (seq_t)next_candidate;
                ++cached_len;
                ++next_candidate;               /* move past the value we just added */
                break;
            }
            ++next_candidate;
        }
    }

    return cache[(size_t)n - 1];
}
