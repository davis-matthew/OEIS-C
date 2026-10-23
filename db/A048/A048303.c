/*
 * A048303  Numbers whose base-6 expansions, read from left to right, have run
 *          lengths that strictly increase.
 * https://oeis.org/A048303
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth integer whose base‑6 expansion, read from left
 *            to right, has run lengths that strictly increase. The first few
 *            terms are 0,1,2,3,4,5,7,14,
 * Range:     u8 n <= 41, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>


static bool is_valid(seq_t val)
{
    if (val == 0)
        return true;

    /* Convert to base‑6 digits (least significant first). */
    unsigned char digits[4096];
    size_t len = 0;
    seq_t tmp = val;

    while (tmp > 0) {
        digits[len++] = (unsigned char)(tmp % 6);
        tmp /= 6;
    }

    /* Scan runs from most significant to least significant digit. */
    unsigned char prev_digit = digits[len - 1];
    size_t curr_len = 1;
    size_t prev_len = 0;           /* no previous run yet */

    for (ptrdiff_t i = (ptrdiff_t)len - 2; i >= 0; --i) {
        if (digits[i] == prev_digit) {
            ++curr_len;
        } else {
            if (curr_len <= prev_len)
                return false;
            prev_len = curr_len;
            prev_digit = digits[i];
            curr_len = 1;
        }
    }

    if (curr_len <= prev_len)
        return false;

    return true;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap   = 0;
    static seq_t next_candidate = 0;

    while (cache_size <= (size_t)n) {
        if (is_valid(next_candidate)) {
            if (cache_cap == 0) {
                cache_cap = 16;
                cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
            } else if (cache_size >= cache_cap) {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_size++] = next_candidate;
        }
        ++next_candidate;
    }

    return cache[(size_t)n];
}
