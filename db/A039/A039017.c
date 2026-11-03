/*
 * A039017  Numbers whose base-6 representation has the same number of 0's and
 *          2's.
 * https://oeis.org/A039017
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 110, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 80 terms through n = 9191 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence of numbers whose base-6 representation has the same number of
   0's and 2's. Computed by scanning integers and selecting those that
   satisfy the condition. */
#include <stdbool.h>

/* Test whether x has equal number of 0 and 2 digits in base‑6. */
static bool has_equal_zero_two(seq_t x)
{
    unsigned c0 = 0, c2 = 0;

    /* 0 itself does not satisfy the condition. */
    if (x == 0)
        return false;

    while (x != 0) {
        unsigned digit = (unsigned)(x % 6);
        if (digit == 0)  ++c0;
        else if (digit == 2) ++c2;
        x /= 6;
    }
    return c0 == c2;
}

seq_t a(idx_t n)
{
    idx_t found = 0;
    seq_t cur  = 1;  /* start from 1 – 0 is not part of the sequence */

    while (found < n) {
        if (has_equal_zero_two(cur)) {
            ++found;
            if (found == n)
                return cur;
        }
        ++cur;   /* next candidate */
    }

    /* Should never reach here for n >= 1 */
    return 0;
}
