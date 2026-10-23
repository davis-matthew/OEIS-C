/*
 * A039034  Numbers whose base-7 representation has the same number of 0's and
 *          5's.
 * https://oeis.org/A039034
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 145, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute numbers whose base-7 representation has the same number of 0's and 5's.
   The function searches the integers in increasing order and returns the nth such
   number (1-based indexing).  This implements the definition of A039034.  */

#include <stdbool.h>

static bool equal_zero_five(seq_t x)
{
    int cnt0 = 0, cnt5 = 0;
    seq_t y = x;
    if (y == 0) {
        /* The number 0 has one zero digit and no fives, so it does not satisfy
           the condition. */
        return false;
    }
    do {
        seq_t d = y % 7;
        if (d == 0) cnt0++;
        else if (d == 5) cnt5++;
        y /= 7;
    } while (y > 0);
    return cnt0 == cnt5;
}

seq_t a(idx_t n)
{
    idx_t count = 0;       /* Count of qualifying numbers found. */
    seq_t val = 1;         /* Current integer being tested. */

    for (;;) {
        if (equal_zero_five(val)) {
            ++count;
            if (count == n) {
                return val;
            }
        }
        ++val;
    }
}
