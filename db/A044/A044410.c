/*
 * A044410  Numbers n such that string 7,8 occurs in the base 10
 *          representation of n but not of n-1.
 * https://oeis.org/A044410
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Validated: partial, pass, 39 terms through n = 39 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers n whose decimal representation contains the substring "78"
   but n-1 does not.  Offset 1. */
#include <stdbool.h>

static bool has78(seq_t x)
{
    seq_t d1, d2;
    while (x >= (seq_t)10) {
        d1 = x % (seq_t)10;
        d2 = (x / (seq_t)10) % (seq_t)10;
        if (d1 == (seq_t)8 && d2 == (seq_t)7)
            return true;
        x /= (seq_t)10;
    }
    return false;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (seq_t cur = 1;; ++cur) {
        if (has78(cur) && !has78(cur - (seq_t)1)) {
            ++count;
            if (count == n)
                return cur;
        }
    }
}
