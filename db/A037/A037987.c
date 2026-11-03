/*
 * A037987  Numbers whose maximal base-6 run length is 4.
 * https://oeis.org/A037987
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 37, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Validated: partial, pass, 37 terms through n = 37 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A037987: numbers whose maximal base-6 run length is 4.
   Sequence is computed by brute-force enumeration. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Return true iff the maximal run of equal base‑6 digits in num is exactly 4. */
static bool has_max_run4(seq_t num)
{
    if (num == (seq_t)0)          /* 0 has no runs of length 4 */
        return false;

    seq_t tmp   = num;
    seq_t prev  = (seq_t)6;       /* sentinel not a valid digit */
    int  run    = 0;
    int  max_run= 0;

    while (tmp > (seq_t)0) {
        seq_t digit = tmp % (seq_t)6;
        tmp /= (seq_t)6;

        if (digit == prev) {
            run++;
        } else {
            prev = digit;
            run  = 1;
        }
        if (run > max_run) {
            max_run = run;
            if (max_run > 4) /* early exit if we exceeded 4 */
                break;
        }
    }
    return max_run == 4;
}

/* Return the nth term (1‑based) of sequence A037987. */
seq_t a(idx_t n)
{
    /* Cache previously computed terms for fast repeated access. */
    static seq_t *cache = NULL;
    static size_t  cap   = 0;
    static size_t  count = 0;
    static seq_t  next_candidate = (seq_t)1;

    /* Generate terms until we have at least n of them. */
    while (count < n) {
        /* Skip numbers that do not satisfy the property. */
        while (!has_max_run4(next_candidate))
            next_candidate++;

        /* Grow the cache if necessary. */
        if (count >= cap) {
            size_t new_cap = (cap == 0) ? 64 : cap * 2;
            seq_t *tmp = mem_resize(seq_t, cache, new_cap);
            if (!tmp)          /* allocation failed */
                return (seq_t)0;
            cache = tmp;
            cap   = new_cap;
        }

        cache[count++] = next_candidate;
        next_candidate++;
    }

    return cache[n - 1];
}
