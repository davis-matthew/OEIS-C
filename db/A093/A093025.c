/*
 * A093025  Indices of terms in A093019 with value 5.
 * https://oeis.org/A093025
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 6553, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 70 terms through n = 9900 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A093025 – indices of terms in A093019 with value 5.
   a(n) = the nth integer whose Luhn check digit equals 5. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Compute the weighted sum of the digits of `num` as they would appear
   in a Luhn calculation *before* the check digit is appended.
   The right‑most digit of `num` is doubled, the next not doubled,
   and so on, because the future check digit will occupy the units
   place and is never doubled.  The function returns the sum modulo 10. */
static int
luhn_prefix_sum(idx_t num)
{
    int sum = 0;
    int double_next = 1;          /* the rightmost digit of `num` gets doubled */
    while (num > 0) {
        int d = num % 10;
        if (double_next) {
            d *= 2;
            if (d > 9) d -= 9;
        }
        sum += d;
        double_next ^= 1;
        num /= 10;
    }
    return sum % 10;              /* we only need the modulo 10 value */
}

/* Return the nth term of A093025.  The sequence starts at offset 1. */
seq_t
a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t next_candidate = 1;   /* next number to test */

    /* Need to generate more terms? */
    while (cache_len < (size_t)n) {
        int sum_mod = luhn_prefix_sum(next_candidate);
        int required_digit = (10 - sum_mod) % 10;
        if (required_digit == 5) {
            /* Store the term, extending the cache if necessary */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap == 0 ? 64 : cache_cap * 2;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) return 0;           /* allocation failure – not expected */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }

    /* n is 1‑based; array is 0‑based */
    return cache[(size_t)n - 1];
}
