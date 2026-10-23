/*
 * A011531  Numbers that contain a digit 1 in their decimal representation.
 * https://oeis.org/A011531
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = smallest integer m ≥ a(n‑1)+1 whose decimal
 *            representation contains the digit 1 (a(1) = 1)
 * Range:     u8 n <= 134, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

/* Check whether the decimal representation of x contains the digit 1. */
static bool has_digit_one(seq_t x)
{
    if (x == (seq_t)0) return false;
    while (x != (seq_t)0) {
        seq_t d = x % (seq_t)10;
        if (d == (seq_t)1) return true;
        x /= (seq_t)10;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* cache for previously computed terms */
    static seq_t  *cache      = NULL;
    static idx_t  cache_len   = 0;
    static idx_t  cache_cap   = 0;
    static seq_t  last_checked = 0;   /* largest integer checked so far */

    if (n == 0) return (seq_t)0;  /* undefined for n<1, return 0 safely */

    /* Ensure the cache holds at least n terms. */
    while (cache_len < n) {
        /* Move to the next candidate integer. */
        last_checked += (seq_t)1;
        if (has_digit_one(last_checked)) {
            /* Grow the cache if necessary. */
            if (cache_len >= cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 4;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();          /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = last_checked;
        }
    }
    return cache[n - 1];
}
