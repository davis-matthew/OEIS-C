/*
 * A045024  Numbers whose base-4 representation contains exactly two 0's and
 *          one 1.
 * https://oeis.org/A045024
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 1700, u32 n <= 1700, u64 n <= 1700,
 *            u128 n <= 1700, big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose base‑4 representation contains exactly two 0's and one 1. */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/seq_realloc.h"

/* Test whether a number has exactly two zeros and one one in base 4. */
static bool is_valid(seq_t x)
{
    if (x < 16)                     /* 16 = 100_4 is the smallest instance */
        return false;
    unsigned zeros = 0, ones = 0;
    while (x) {
        seq_t d = x % 4;
        if (d == 0)
            zeros++;
        else if (d == 1)
            ones++;
        x /= 4;
    }
    return zeros == 2 && ones == 1;
}

/* Memoisation buffer for already found terms. */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static seq_t next_check = 0;

seq_t a(idx_t n)
{
    /* Offset is 1, but guard against n == 0 */
    if (n == 0)
        return 0;

    /* Extend cache until we have at least n terms. */
    while (cache_len < (size_t)n) {
        if (is_valid(next_check)) {
            /* Grow the array if necessary. */
            if (cache_len >= cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                cache = seq_realloc(cache, cache_cap, new_cap);
                cache_cap = new_cap;
            }
            cache[cache_len++] = next_check;
        }
        next_check++;                 /* Move to the next candidate integer. */
    }

    /* Return the (n‑1)-th stored term (array is 0‑based). */
    return cache[(size_t)(n - 1)];
}
