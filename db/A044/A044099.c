/*
 * A044099  Numbers n such that string 4,2 occurs in the base 5 representation
 *          of n but not of n-1.
 * https://oeis.org/A044099
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 62 terms through n = 872 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth element of OEIS sequence A044099:
   Numbers n such that the string 4,2 occurs in the base‑5
   representation of n but not in that of n‑1.
   The value a(1) is 22. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Helper: true if the base‑5 representation of x contains the
   consecutive digits 4 followed by 2 (most significant → least). */
static bool has_42_base5(seq_t x)
{
    int prev = -1;          /* previous digit, most significant seen so far */
    while (x > 0) {
        int cur = (int)(x % 5);
        if (prev == 2 && cur == 4) {           /* pattern 2,4 when scanning LSB→MSB */
            return true;
        }
        prev = cur;
        x /= 5;
    }
    return false;
}

/* Required external function */
seq_t a(idx_t n)
{
    /* Static state to cache results across calls */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap  = 0;
    static seq_t last_checked = 0;

    /* Extend cache until it contains at least n elements */
    while (cache_size < n) {
        /* Next integer to test */
        seq_t cand = last_checked + 1;
        last_checked = cand;

        if (has_42_base5(cand) && !has_42_base5(cand - 1)) {
            /* Ensure capacity */
            if (cache_size == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 32;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) exit(EXIT_FAILURE);
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = cand;
        }
    }

    return cache[n - 1];
}
