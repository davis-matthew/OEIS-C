/*
 * A044393  Numbers n such that string 6,1 occurs in the base 10
 *          representation of n but not of n-1.
 * https://oeis.org/A044393
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth positive integer such that the decimal representation
 *            of n contains the substring "61" while the decimal
 *            representation of n-1 does not
 * Range:     u8 n <= 2, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Validated: partial, pass, 39 terms through n = 39 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool contains_61(idx_t x)
{
    if (x == 0)
        return false;
    idx_t prev = 10;        /* sentinel for “no previous digit”   */
    while (x > 0) {
        idx_t cur = x % 10; /* least‑significant digit            */
        if (cur == 6 && prev == 1)   /* detects "61" in forward order   */
            return true;
        prev = cur;
        x /= 10;
    }
    return false;
}

seq_t a(idx_t n)
{
    idx_t count = 0;        /* how many qualifying numbers found   */
    idx_t x     = 1;        /* candidate integer to test           */

    while (true) {
        if (contains_61(x) && !contains_61(x - 1)) {
            ++count;
            if (count == n)
                return (seq_t)x;
        }
        ++x;
    }
}
