/*
 * A171902  First differences of consecutive numbers having at least two
 *          identical neighboring digits in decimal representation.
 * https://oeis.org/A171902
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_adjacent_dup
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*

   Computes a(n) = A171901(n+1) – A171901(n),
   where A171901(k) is the k‑th positive integer that contains
   at least one pair of adjacent equal decimal digits.  The
   values are returned as the difference between successive
   members of that set (first value is 11).

   The function memoises results so that calls with arbitrary
   indices are answered in O(1) after the required difference
   has been generated.  The implementation tests each number
   for adjacent repeated digits and records the differences.

*/

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/has_adjacent_dup.h"

seq_t a(idx_t n)
{
    /* Static state for memoisation */
    static seq_t *diffs = NULL;      /* 1‑based array of differences */
    static size_t diff_cap = 0;      /* capacity of diffs array */
    static idx_t  diff_len = 0;      /* number of differences computed (>=0) */

    static seq_t last_tested = 0;    /* last candidate examined */
    static seq_t prev_number = 0;    /* previous A171901 value found */
    static idx_t  found_count = 0;   /* count of numbers found so far */

    /* Ensure we have computed at least n differences. */
    while (diff_len < n) {
        /* Find the next number that has adjacent duplicate digits. */
        for (;;) {
            ++last_tested;
            if (has_adjacent_dup(last_tested))
                break;
        }

        ++found_count;
        if (found_count == 1) {
            /* First number found – no difference yet. */
            prev_number = last_tested;
        } else {
            /* Compute difference between this and previous number. */
            seq_t d = last_tested - prev_number;

            /* Grow the array if needed; we keep index 0 unused. */
            if ((size_t)(diff_len + 1) > diff_cap) {
                size_t new_cap = diff_cap ? diff_cap * 2 : 16;
                if (new_cap < (size_t)(diff_len + 1))
                    new_cap = diff_len + 1;
                diff_cap = new_cap;
                seq_t *tmp = (seq_t *)realloc(diffs, (diff_cap + 1) * sizeof(seq_t));
                if (!tmp) abort();   /* memory allocation failed */
                diffs = tmp;
            }

            diffs[diff_len + 1] = d;  /* store at 1‑based index */
            ++diff_len;

            prev_number = last_tested;
        }
    }

    /* n is 1‑based, so return the stored value at that index. */
    return diffs[n];
}
