/*
 * A055052  Numbers of the form 4^i*(8j+7) or 4^i*(8j+5).
 * https://oeis.org/A055052
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 84, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 72 terms through n = 9282 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Predicate: integer m is of the form 4^i*(8j+5) or 4^i*(8j+7). */

#include <stdbool.h>

static bool is_desired(seq_t n)
{
    // Compute exponent of 2 (v2) by stripping off factors of 2
    seq_t tmp = n;
    idx_t v2 = 0;
    while ((tmp & 1) == 0) {
        tmp >>= 1;
        v2++;
    }
    // v2 must be even; otherwise the number cannot be written as 4^i * odd
    if (v2 & 1)
        return false;

    // tmp is now the odd part.  It must be congruent to 5 or 7 mod 8.
    return ((tmp & 7) == 5) || ((tmp & 7) == 7);
}

seq_t a(idx_t n)
{
    /* Offsets are 1-based: a(1) = 5. */
    seq_t cur = 1;
    idx_t count = 0;
    while (count < n) {
        if (is_desired(cur))
            ++count;
        ++cur;
    }
    return cur - 1;
}
