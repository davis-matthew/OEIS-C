/*
 * A121610  Composite numbers that can be written as concatenation of two
 *          composite numbers in decimal representation.
 * https://oeis.org/A121610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 53, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   digits, is_prime_small, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: enumerate composite numbers and keep those that can be split into two composite parts. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "../utils/is_prime_small.h"
#include "../utils/digits.h"
#include "../utils/pow10.h"

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t next_candidate = 4;

/* Test whether a seq_t number is composite. */
static inline bool is_composite(seq_t x) {
    if (x <= (seq_t)3) return false;
    return !is_prime_small(x);
}

/* Return true if num can be split into two composite parts. */
static bool has_two_composite_parts(seq_t num) {
    int d = digits((idx_t)num);            /* number of decimal digits */
    for (int pos = 1; pos < d; ++pos) {
        seq_t pow = pow10((idx_t)(d - pos));
        seq_t left = num / pow;
        seq_t right = num % pow;
        if (is_composite(left) && is_composite(right))
            return true;
    }
    return false;
}

static void grow_cache(size_t need) {
    if (need > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < need) new_cap = need;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* undefined: return 0 for safety */
    while (cache_len < (size_t)n) {
        /* Search for the next qualifying composite number */
        while (true) {
            seq_t cand = (seq_t)next_candidate;
            ++next_candidate;    /* move to next integer for future searches */
            if (cand >= (seq_t)4 && is_composite(cand) && has_two_composite_parts(cand)) {
                grow_cache(cache_len + 1);
                cache[cache_len++] = cand;
                break;
            }
        }
    }
    return cache[(size_t)n - 1];
}
