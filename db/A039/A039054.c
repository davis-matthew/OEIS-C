/*
 * A039054  Numbers whose base-8 representation has the same number of 0's and
 *          5's.
 * https://oeis.org/A039054
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Counts numbers whose base‑8 representation has the same number of 0’s and 5’s.
   The function returns a(n) with offset 1. */
#include <stdbool.h>

static bool equal_zero_five(seq_t x)
{
    int c0 = 0, c5 = 0;
    while (x != 0) {
        unsigned digit = (unsigned)(x % 8);
        if (digit == 0)
            ++c0;
        else if (digit == 5)
            ++c5;
        x = x / 8;
    }
    return c0 == c5;
}

seq_t a(idx_t n)
{
    seq_t num = 1;
    idx_t count = 0;
    /* Linear search until the n‑th qualifying number is found. */
    while (true) {
        if (equal_zero_five(num)) {
            ++count;
            if ((idx_t)count == n)
                return num;
        }
        ++num;
    }
}
