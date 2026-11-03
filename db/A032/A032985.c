/*
 * A032985  Numbers with the property that all pairs of consecutive base-8
 *          digits differ by more than 2.
 * https://oeis.org/A032985
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth positive integer whose base‑8 representation has
 *            no two consecutive digits differing by 2 or less
 * Range:     u8 n <= 56, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Return true if val has no adjacent base‑8 digits with absolute difference <= 2. */
static bool valid_base8(seq_t val)
{
    bool first = true;
    seq_t prev_digit = 0;

    while (val > 0) {
        seq_t digit = val % 8;
        if (!first) {
            seq_t diff = prev_digit > digit ? prev_digit - digit : digit - prev_digit;
            if (diff <= 2)
                return false;
        }
        prev_digit = digit;
        first = false;
        val /= 8;
    }
    return true;
}

seq_t a(idx_t n)
{
    seq_t target = (seq_t)n;
    seq_t count = 0;
    seq_t cand = 0;

    while (count < target) {
        ++cand;
        if (valid_base8(cand))
            ++count;
    }
    return cand;
}
