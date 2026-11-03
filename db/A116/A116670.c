/*
 * A116670  Numbers with all but one decimal digit.
 * https://oeis.org/A116670
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 23, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Validated: partial, pass, 23 terms through n = 23 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Computation of OEIS sequence A116670:
 * a(n) = the n-th smallest integer whose decimal representation
 * contains exactly nine distinct digits (the missing digit may be
 * any of 0..9). Leading zeros are not permitted.
 * The code below does not use a closed formula; instead it
 * increments from the first such number (102345678) and counts
 * qualifying numbers until the desired index is reached.
 */

#include <stdbool.h>

/* Helper: return true iff x has exactly nine distinct decimal digits */
static bool has_nine_distinct_digits(seq_t x)
{
    unsigned mask = 0;        /* bit mask for digits 0-9 */
    while (x > 0) {
        unsigned digit = (unsigned)(x % 10);
        mask |= 1U << digit;
        x /= 10;
    }
    /* Count bits set in mask */
    int count = 0;
    while (mask) {
        count++;
        mask &= mask - 1;
    }
    return count == 9;
}

/* External function required by the build */
seq_t a(idx_t n)
{
    /* The first term is fixed: 102345678 */
    seq_t start = 102345678;
    idx_t count = 0;
    seq_t cur   = start;

    /* Scan integers upward until the n-th qualifying number is found */
    while (true) {
        if (has_nine_distinct_digits(cur)) {
            ++count;
            if (count == n)
                return cur;
        }
        ++cur;
    }
}
