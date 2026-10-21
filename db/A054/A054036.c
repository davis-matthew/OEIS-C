/*
 * A054036  Numbers n such that n^2 contains exactly 8 different digits.
 * https://oeis.org/A054036
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 5864, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_distinct_decimal_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A054036: a(n) = the n‑th integer such that n² contains
   exactly 8 distinct decimal digits.  The sequence is generated
   by brute‑force search, testing each integer until the required
   number of terms have been found. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/count_distinct_decimal_digits.h"

/* ----- cache for already computed terms of the sequence ----- */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;

/* Append a term to the cache, expanding it if necessary. */
static void append_term(seq_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? 2 * cache_cap : 16;
        seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = new_arr;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* Main sequence function.  It keeps a static counter and a cache
   so that calls in any order are handled correctly. */
seq_t a(idx_t n)
{
    /* Ensure the cache holds at least n terms. */
    static idx_t last_checked = 0;   /* last integer examined for the property */
    while (cache_len < n) {
        ++last_checked;
        seq_t sq = (seq_t)last_checked * (seq_t)last_checked;
        if (count_distinct_decimal_digits(sq) == 8) {
            append_term(last_checked);
        }
    }
    return cache[n - 1];
}
