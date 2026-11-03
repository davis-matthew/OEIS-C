/*
 * A044752  Numbers n such that string 3,9 occurs in the base 10
 *          representation of n but not of n+1.
 * https://oeis.org/A044752
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Validated: partial, pass, 39 terms through n = 39 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044752: a(n) is the n-th integer k such that the decimal
   representation of k contains the substring "39" but k+1 does not. */

#include <stdbool.h>

/* Private helper: returns true if x contains the adjacent digits 3 followed by 9 */
static bool has39(seq_t x)
{
    seq_t prev = 10; /* impossible digit value to start */
    while (x > 0) {
        seq_t d = x % 10;
        if (d == 3 && prev == 9)
            return true;
        prev = d;
        x /= 10;
    }
    return false;
}

seq_t a(idx_t n)
{
    idx_t cnt = 0;
    for (seq_t k = 1; ; ++k) {
        if (has39(k) && !has39(k + 1)) {
            ++cnt;
            if (cnt == n)
                return k;
        }
    }
}
