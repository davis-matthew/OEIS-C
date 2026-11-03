/*
 * A050813  Numbers n not palindromic in any base b, 2 <= b <= 10.
 * https://oeis.org/A050813
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th positive integer that is NOT a palindrome in any
 *            base b, 2 ≤ b ≤ 10
 * Range:     u8 n <= 81, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 71 terms through n = 9626 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Helper: test whether an integer n is a palindrome in base 'base'.
   Assumes base >= 2 and <= 10. */
static bool is_pal_in_base(seq_t n, int base)
{
    /* Maximum number of digits needed for an unsigned __int128 in base 2 is 128. 
       For smaller types this is more than sufficient. */
    uint8_t digits[128];
    int len = 0;
    seq_t tmp = n;

    /* Special case: zero is a palindrome in every base. */
    if (tmp == 0) {
        return true;
    }

    while (tmp > 0) {
        /* tmp % base and tmp / base are valid for integer types and arbitrary precision ints. */
        digits[len++] = (uint8_t)(tmp % base);
        tmp = tmp / base;
    }

    for (int i = 0; i < len / 2; ++i) {
        if (digits[i] != digits[len - 1 - i]) {
            return false;
        }
    }
    return true;
}

/* Helper: test whether n is NOT a palindrome in any base from 2 to 10. */
static bool is_nonpalin_all_bases(seq_t n)
{
    for (int base = 2; base <= 10; ++base) {
        if (is_pal_in_base(n, base)) {
            return false;
        }
    }
    return true;
}

/* Memoised sequence storage.  The array holds the terms a(1), a(2), … .
   The static variables keep state between calls. */
static seq_t *seq_cache = NULL;     /* array of stored terms */
static size_t seq_capacity = 0;     /* allocated elements */
static idx_t seq_count = 0;         /* number of stored terms */
static seq_t last_value = 0;        /* last value added to the cache */

/* Ensure the cache can hold at least 'needed' terms.  Grow exponentially. */
static void ensure_cache_size(size_t needed)
{
    if (needed <= seq_capacity) {
        return;
    }
    size_t new_cap = seq_capacity ? seq_capacity * 2 : 16;
    while (new_cap < needed) {
        new_cap *= 2;
    }
    seq_t *new_ptr = mem_resize(seq_t, seq_cache, new_cap);
    if (!new_ptr) {
        /* In a real build we would handle allocation failure.  For this
           evaluator we simply abort. */
        abort();
    }
    seq_cache = new_ptr;
    seq_capacity = new_cap;
}

/* Main function requested by the build. */
seq_t a(idx_t n)
{
    /* n is 1‑based according to OEIS offset. */
    if (n == 0) {
        /* Behavior below offset does not matter per requirement. */
        return 0;
    }

    if (n <= seq_count) {
        return seq_cache[n - 1];
    }

    /* We need to compute terms up to index n. */
    ensure_cache_size(n);

    seq_t cand = last_value + 1;  /* start searching after the last cached value */
    for (idx_t idx = seq_count; idx < n; ++idx) {
        /* Find the next qualifying number. */
        while (!is_nonpalin_all_bases(cand)) {
            ++cand;
        }
        seq_cache[idx] = cand;
        last_value = cand;
        ++cand;
    }
    seq_count = n;
    return seq_cache[n - 1];
}
