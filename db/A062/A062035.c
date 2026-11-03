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
 * Validated: standard, pass, 61 terms through n = 498 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

/* Check if a 64‑bit integer satisfies the property */
static inline int is_valid_uint64(uint64_t x) {
    uint64_t sum = 0;
    __int128 prod = 1;
    while (x) {
        uint8_t d = x % 10;
        if (d == 0) return 0;          /* digit zero -> product zero */
        sum += d;
        prod *= d;
        x /= 10;
    }
    if (sum == 0) return 0;
    return prod == (__int128)3 * sum;
}

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
                    cache = mem_resize(seq_t, cache, newcap);
                    cache_cap = newcap;
                }
                cache[cache_len++] = (seq_t)val;
                break;
            }
        }
    }
    return cache[(size_t)n - 1];
}
