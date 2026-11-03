/*
 * A044739  Numbers n such that string 2,6 occurs in the base 10
 *          representation of n but not of n+1.
 * https://oeis.org/A044739
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth integer such that the decimal representation of n
 *            contains the substring "26" but the decimal representation of
 *            n+1 does not. The implementation searches sequentially from 26
 *            upward until n terms are found
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Validated: partial, pass, 39 terms through n = 39 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static inline bool has26(seq_t x)
{
    /* Returns true if decimal representation of x contains "26". */
    seq_t prev = 10;            /* sentinel – any value not 2 or 6 */
    while (x > 0) {
        seq_t d = x % 10;
        if (d == 2 && prev == 6) return true;   /* ...26... detected */
        prev = d;
        x /= 10;
    }
    return false;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (seq_t x = (seq_t)26; ; ++x) {
        if (has26(x) && !has26(x + 1)) {
            ++count;
            if (count == n) return x;
        }
    }
}
