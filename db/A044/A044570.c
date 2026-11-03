/*
 * A044570  Numbers n such that string 0,2 occurs in the base 8 representation
 *          of n but not of n+1.
 * https://oeis.org/A044570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Validated: partial, pass, 40 terms through n = 40 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: numbers n whose base‑8 representation contains the substring "02"
   while the representation of n+1 does not.  The function a(n) returns the
   n-th such number (with a(1) = 66).  */

#include <stdbool.h>

static bool has_02(seq_t x)
{
    bool seen_prev = false;
    seq_t prev_digit = 0;
    while (x > 0) {
        seq_t digit = x % 8;
        x /= 8;
        if (seen_prev && digit == 0 && prev_digit == 2)
            return true;
        prev_digit = digit;
        seen_prev = true;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* undefined for the problem, return 0 for safety */

    idx_t found = 0;
    for (seq_t x = 1;; ++x) {
        if (has_02(x)) {
            seq_t x1 = x + 1;
            if (!has_02(x1)) {
                ++found;
                if (found == n)
                    return x;
            }
        }
    }
}
