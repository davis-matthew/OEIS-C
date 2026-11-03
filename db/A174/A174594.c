/*
 * A174594  a(n) is the smallest number that is less than the sum of the
 *          square of its digits by n, or 0 if no such number exists.
 * https://oeis.org/A174594
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = smallest x > 0 such that digit_square_sum(x) - x = n, or
 *            0 if no such x exists
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Validated: partial, pass, 73 terms through n = 72 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The maximal n for which a solution can exist is 152, because
   for any integer with 4 or more digits we have
   digit_square_sum(x) ≤ 81·4 = 324 < 10³ = x, so
   digit_square_sum(x) - x < 0.  Thus we only need to search
   1 ≤ x ≤ 999.  All valid a(n) are found by scanning this range
   once and caching the results. */

#include <stdbool.h>
#include <stdlib.h>

/* Helper: sum of squares of decimal digits of a 64‑bit integer. */
static uint64_t digit_square_sum(uint64_t x)
{
    uint64_t sum = 0;
    while (x > 0) {
        uint64_t d = x % 10;
        sum += d * d;
        x /= 10;
    }
    return sum;
}

seq_t a(idx_t n) {
    /* 153 possible values: 0 … 152. */
    static seq_t cache[153];
    static bool init = false;

    if (!init) {
        /* Compute mapping from n -> minimal x. */
        for (idx_t x = 1; x <= 999; ++x) {
            uint64_t diff = digit_square_sum(x) - x;  /* f(x) = S(x) – x */
            if (diff <= 152) {
                idx_t idx = (idx_t)diff;
                if (cache[idx] == 0) {
                    cache[idx] = (seq_t)x;
                }
            }
        }
        init = true;
    }

    if (n > 152) {
        return (seq_t)0;            /* no solution for larger n */
    }
    return cache[(idx_t)n];
}
