/*
 * A062035  Positive numbers whose product of digits is three times their sum.
 * https://oeis.org/A062035
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the nth positive integer x such that product_of_digits(x)
 *            = 3 * sum_of_digits(x)
 * Range:     u8 n <= 7, u16 n <= 149, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   is_valid_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_valid_uint64.h"

/* Cache the sequence */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static uint64_t next_candidate = 1;

/* Return the n‑th term of the sequence */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined, but safe */
    while (cache_len < (size_t)n) {
        /* Search for the next term */
        while (1) {
            uint64_t val = next_candidate++;
            if (is_valid_uint64(val)) {
                /* Ensure capacity */
                if (cache_len == cache_cap) {
                    size_t newcap = cache_cap ? cache_cap * 2 : 16;
                    cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
                    cache_cap = newcap;
                }
                cache[cache_len++] = (seq_t)val;
                break;
            }
        }
    }
    return cache[(size_t)n - 1];
}
