/*
 * A044591  Numbers n such that string 2,7 occurs in the base 8 representation
 *          of n but not of n+1.
 * https://oeis.org/A044591
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth integer n such that '27' occurs in the base‑8
 *            representation of n but not of n+1
 * Range:     u8 n <= 5, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Validated: partial, pass, 41 terms through n = 41 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/mem.h"

/* Check whether base‑8 representation of val contains the
   substring "27" (most significant -> least significant). */
static bool has_substring_27(seq_t val)
{
    /* Base‑8 digits are stored least‑significant first. */
    uint8_t digits[128];
    size_t len = 0;
    seq_t x = val;
    if (x == 0) {
        digits[len++] = 0;
    } else {
        while (x > 0) {
            digits[len++] = (uint8_t)(x % 8);
            x /= 8;
        }
    }

    /* Look for pattern 7 (least significant) followed by 2. */
    for (size_t i = 0; i + 1 < len; ++i) {
        if (digits[i] == 7 && digits[i + 1] == 2)
            return true;
    }
    return false;
}

/* Cached sequence of found terms. */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static seq_t next_test = 1;

/* Append a value to the cache, growing it if needed. */
static void append_to_cache(seq_t val)
{
    if (cache_len >= cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
        seq_t *new_buf = mem_resize(seq_t, cache, new_cap);
        if (!new_buf) abort(); /* memory failure */
        cache = new_buf;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* Main sequence generator. */
seq_t a(idx_t n)
{
    /* Ignore requests below the offset (n < 1). */
    if (n < 1) {
        return 0;
    }

    /* Extend cache until we have at least n terms. */
    while (cache_len < n) {
        seq_t cand = next_test++;
        if (has_substring_27(cand) && !has_substring_27(cand + 1)) {
            append_to_cache(cand);
        }
    }
    return cache[n - 1];
}
