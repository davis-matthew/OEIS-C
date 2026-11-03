/*
 * A044418  Numbers n such that string 8,6 occurs in the base 10
 *          representation of n but not of n-1.
 * https://oeis.org/A044418
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers n whose decimal representation contains the substring
 *            "86" but n-1 does not
 * Range:     u8 n <= 2, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Validated: partial, pass, 39 terms through n = 39 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool has86(seq_t x)
{
    if (x == 0) return false;
    seq_t y = x;
    while (y >= 10) {
        seq_t d2 = y % 10;
        seq_t d1 = (y / 10) % 10;
        if (d2 == 6 && d1 == 8) return true;
        y /= 10;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* undefined for n < 1 */
    idx_t count = 0;
    seq_t cand = 1;
    for (;;) {
        if (has86(cand) && !has86(cand - 1)) {
            ++count;
            if (count == n) return cand;
        }
        ++cand;
    }
}
