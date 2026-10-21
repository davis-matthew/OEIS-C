/*
 * A065214  Numbers which need nine 'Reverse and Add' steps to reach a
 *          palindrome.
 * https://oeis.org/A065214
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers that need exactly nine reverse‑and‑add steps to reach a
 *            palindrome (search)
 * Range:     u8 none, u16 none, u32 n <= 1000, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   is_palindrome, reverse_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_palindrome.h"
#include "../utils/reverse_decimal.h"

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static seq_t next_candidate = 1;

/* Return true if starting from start we need exactly nine steps to reach a palindrome.
   Palindromes themselves and any earlier step terminating in a palindrome are excluded. */
static bool needs9(seq_t start) {
    seq_t cur = start;
    for (int step = 0; step < 9; ++step) {
        if (is_palindrome(cur, 10)) return false;  /* reached before 9 steps or start is palindrome */
        cur = cur + reverse_decimal(cur);
    }
    return is_palindrome(cur, 10);               /* reached at the 9th step */
}

/* Ensure capacity for at least min_cap elements in the cache. */
static void ensure_capacity(size_t min_cap) {
    if (cache_cap >= min_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < min_cap) new_cap = min_cap;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n) {
    /* n is 1‑based; ignore requests for n < 1 */
    if (n == 0) return 0;

    /* Produce terms up to the requested index */
    while (cache_len < n) {
        seq_t val = next_candidate;
        if (needs9(val)) {
            ensure_capacity(cache_len + 1);
            cache[cache_len++] = val;
        }
        ++next_candidate;
    }
    return cache[n - 1];
}
