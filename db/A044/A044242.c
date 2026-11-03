/*
 * A044242  Numbers n such that string 6,7 occurs in the base 8 representation
 *          of n but not of n-1.
 * https://oeis.org/A044242
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   sequence of n such that the string "6,7" occurs in the base‑8
 *            representation of n but not of n‑1
 * Range:     u8 n <= 4, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Validated: partial, pass, 40 terms through n = 40 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Return true if v has no substring 6,7 in its base‑8 representation. */
static bool has_no_67(seq_t v)
{
    seq_t prev = 0;
    bool have_prev = false;
    while (v > 0) {
        seq_t cur = v % 8;
        if (have_prev && cur == 6 && prev == 7)
            return false;                 /* found pattern, so not allowed */
        v = v / 8;
        prev = cur;
        have_prev = true;
    }
    return true;
}

/* Return true if the base‑8 representation of v contains the
   substring 6,7 (most significant digit 6 followed by 7). */
static bool has_substring_67(seq_t v)
{
    seq_t prev = 0;
    bool have_prev = false;
    while (v > 0) {
        seq_t cur = v % 8;                /* least significant digit */
        if (have_prev && cur == 6 && prev == 7)
            return true;                  /* found pattern 6,7 */
        v = v / 8;
        prev = cur;
        have_prev = true;
    }
    return false;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t m = 1;
    for (;;) {
        if (has_substring_67(m) && has_no_67(m - 1)) {
            ++count;
            if (count == n)
                return m;
        }
        ++m;
    }
}
