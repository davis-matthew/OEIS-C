/*
 * A039266  Numbers whose base-12 representation has the same number of 7's
 *          and 9's.
 * https://oeis.org/A039266
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 183, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 84 terms through n = 919 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose base-12 representation has the same number of 7's and 9's. */
#include <stdbool.h>

/* Returns true if the base‑12 digits of x contain an equal number of 7’s and 9’s. */
static inline bool has_equal_7_9(seq_t x)
{
    int c7 = 0;
    int c9 = 0;
    seq_t t = x;
    while (t)
    {
        seq_t d = t % 12;
        if (d == 7)
            ++c7;
        else if (d == 9)
            ++c9;
        t /= 12;
    }
    return c7 == c9;
}

/* a(n) = the n-th non‑negative integer whose base‑12 representation has equal
 * numbers of 7’s and 9’s.  a(1) = 0. */
seq_t a(idx_t n)
{
    seq_t val = 0;
    idx_t count = 0;
    while (1)
    {
        if (has_equal_7_9(val))
        {
            ++count;
            if (count == n)
                return val;
        }
        ++val;   /* wrap‑around is fine for the ranges used in the tests */
    }
}
