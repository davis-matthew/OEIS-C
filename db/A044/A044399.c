/*
 * A044399  Numbers n such that string 6,7 occurs in the base 10
 *          representation of n but not of n-1.
 * https://oeis.org/A044399
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers n such that decimal representation contains "67" but n-1
 *            does not
 * Range:     u8 n <= 2, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Validated: partial, pass, 39 terms through n = 39 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Helper: returns true if x contains substring "67" in its decimal representation. */
static bool contains_67(seq_t x)
{
    while (x >= 10) {
        if ((x % 100) == 67)
            return true;
        x /= 10;
    }
    return false;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t cur = 1;
    while (1) {
        if (contains_67(cur) && !contains_67(cur - 1)) {
            ++count;
            if (count == n)
                return cur;
        }
        ++cur;
    }
}
