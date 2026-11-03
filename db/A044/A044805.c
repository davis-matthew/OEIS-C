/*
 * A044805  Numbers k such that string 9,2 occurs in the base 10
 *          representation of k but not of k+1.
 * https://oeis.org/A044805
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

/* OEIS A044805: numbers k such that the decimal string of k contains
   the substring "92" while the decimal string of k+1 does not.  The
   function searches successive integers until it encounters the n‑th
   such number.  Complexity is O(n) with a small constant. */

#include <stdbool.h>

/* Check whether the decimal representation of x contains the
   substring "92". */
static inline bool has92(seq_t x)
{
    seq_t prev = x % 10;
    x /= 10;
    while (x) {
        seq_t cur = x % 10;
        if (cur == 9 && prev == 2)
            return true;
        prev = cur;
        x /= 10;
    }
    return false;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t k = 92;          /* first possible candidate */

    for (;;) {
        if (has92(k)) {
            seq_t kp1 = k + 1;
            if (!has92(kp1)) {
                count++;
                if (count == n)
                    return k;
            }
        }
        k++;
    }
}
