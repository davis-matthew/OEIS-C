/*
 * A206286  Nonprime numbers starting with a digit 1.
 * https://oeis.org/A206286
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 54 terms through n = 54 (2026-11-01)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the n‑th composite number (including 1) whose
   decimal representation begins with the digit 1. */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

/* Return true if the most significant decimal digit of x is 1. */
static inline bool leading_digit_is_one(idx_t x)
{
    while (x >= 10) x /= 10;
    return x == 1;
}

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
