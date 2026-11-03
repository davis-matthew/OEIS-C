/*
 * A037995  Numbers whose maximal base-8 run length is 4.
 * https://oeis.org/A037995
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Validated: partial, pass, 36 terms through n = 36 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers whose maximal base‑8 run length is exactly 4 */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Check whether the largest run of equal digits in the octal
   representation of x equals exactly 4. */
static bool has_run_length4(seq_t x)
{
    if (x == 0)          /* 0 has run length 0 */
        return false;

    seq_t tmp = x;       /* operate on a copy */
    int prev = -1;
    int cur_run = 0;
    int max_run = 0;

    while (tmp != 0) {
        int digit = (int)(tmp % 8);  /* 0–7 */

        if (digit == prev) {
            ++cur_run;
        } else {
            cur_run = 1;
            prev = digit;
        }

        if (cur_run > max_run)
            max_run = cur_run;

        if (max_run > 4)          /* early exit if > 4 */
            return false;

        tmp /= 8;
    }
    return max_run == 4;
}

/* Dynamically cached array of terms */
static seq_t *cache      = NULL;
static size_t cache_cap  = 0;
static size_t cache_len  = 0;
static seq_t next_candidate = 1;

/* Return the n‑th term of the sequence.   (n >= 1) */
seq_t a(idx_t n)
{
    while (cache_len < n) {
        /* Find the next integer satisfying the property. */
        while (!has_run_length4(next_candidate))
            ++next_candidate;

        /* Grow cache if necessary. */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 8;
            seq_t *tmp = mem_resize(seq_t, cache, new_cap);
            if (!tmp) abort();
            cache = tmp;
            cache_cap = new_cap;
        }

        cache[cache_len++] = next_candidate;
        ++next_candidate;   /* next test will start here */
    }

    return cache[n - 1];
}
