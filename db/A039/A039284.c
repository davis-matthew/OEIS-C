/*
 * A039284  Numbers whose base-5 representation has the same nonzero number of
 *          0's and 2's.
 * https://oeis.org/A039284
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS formula: a(n) = nth integer x such that the base-5 representation of x contains
   the same nonzero number of 0’s and 2’s.  The count must be positive. */

#include <stdbool.h>

static bool good(seq_t x)
{
    unsigned long long cnt0 = 0;
    unsigned long long cnt2 = 0;
    seq_t y = x;
    while (y > (seq_t)0) {
        int d = (int)(y % (seq_t)5);
        if (d == 0)
            ++cnt0;
        else if (d == 2)
            ++cnt2;
        y = y / (seq_t)5;
    }
    return (cnt0 == cnt2) && (cnt0 > 0);
}

seq_t a(idx_t n)
{
    idx_t found = 0;
    seq_t candidate = (seq_t)1;
    while (1) {
        if (good(candidate)) {
            ++found;
            if (found == n)
                return candidate;
        }
        ++candidate;
    }
}
