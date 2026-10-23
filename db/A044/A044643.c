/*
 * A044643  Numbers k such that string 1,2 occurs in the base 9 representation
 *          of k but not of k+1.
 * https://oeis.org/A044643
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = nth integer k such that the base‑9 representation of k
 * contains the substring "12" but the base‑9 representation of k+1 does NOT
 * contain "12".  Offset: 1
 */
#include <stdbool.h>

static inline bool has12(seq_t x)
{
    if (x == 0) return false;
    int prev = (int)(x % 9);
    seq_t y = x / 9;
    while (y > 0) {
        int cur = (int)(y % 9);
        if (cur == 1 && prev == 2) return true;
        prev = cur;
        y /= 9;
    }
    return false;
}

seq_t a(idx_t n)
{
    idx_t found = 0;
    seq_t k = (seq_t)1;
    while (found < n) {
        if (has12(k) && !has12(k + 1)) {
            ++found;
            if (found == n) return k;
        }
        ++k;
    }
    return (seq_t)0;  /* unreachable */
}
