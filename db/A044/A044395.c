/*
 * A044395  Numbers n such that string 6,3 occurs in the base 10
 *          representation of n but not of n-1.
 * https://oeis.org/A044395
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: iterate over positive integers, counting those where the decimal
 * representation contains the substring "63" while the previous integer does
 * not. Returns the nth such integer. */

#include <stdbool.h>

/* Check if a decimal number contains the adjacent digits 6 followed by 3
 * (i.e. the substring "63"). */
static bool has63(seq_t x)
{
    bool first = true;
    seq_t last = 0;

    while (x > 0) {
        seq_t cur = x % 10;
        if (!first) {
            if (last == (seq_t)3 && cur == (seq_t)6)
                return true;
        }
        last = cur;
        first = false;
        x = x / 10;
    }
    return false;
}

/* Return the nth number such that the decimal representation of n
 * contains "63" but that of n-1 does not.  The sequence is indexed
 * with a(1) the first such number. */
seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t x = 1;
    seq_t one = 1;

    while (count < n) {
        if (has63(x) && !has63(x - one)) {
            ++count;
            if (count == n)
                return x;
        }
        x = x + one;
    }
    /* For completeness; this line should never be reached for n >= 1. */
    return x;
}
