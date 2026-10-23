/*
 * A044287  Numbers n such that string 4,0 occurs in the base 9 representation
 *          of n but not of n-1.
 * https://oeis.org/A044287
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = n-th integer k such that the string “4,0” occurs in the base‑9 representation of k but not in the base‑9 representation of k‑1. */

#include <stdbool.h>

static bool has_40(seq_t x)
{
    if (x == 0)
        return false;
    seq_t prev = x % 9;
    x /= 9;
    while (x > 0) {
        seq_t cur = x % 9;
        if (prev == 0 && cur == 4)
            return true;
        prev = cur;
        x /= 9;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* Undefined for offset 1; guard against accidental use.  */
    idx_t count = 0;
    seq_t cur = 1;
    while (1) {
        if (has_40(cur) && !has_40(cur - 1)) {
            ++count;
            if (count == n)
                return cur;
        }
        ++cur;
    }
}
