/*
 * A044129  Numbers n such that string 4,3 occurs in the base 6 representation
 *          of n but not of n-1.
 * https://oeis.org/A044129
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044129: a(n) is the nth positive integer whose base‑6
   representation contains the adjacent digits 4,3 but the representation
   of n–1 does not. */

#include <stdbool.h>

/* Returns true if the base‑6 representation of x contains the
   substring 4,3 (digits 4 immediately followed by 3). */
static bool has43(seq_t x)
{
    seq_t prev = 0;
    while (x > 0) {
        seq_t d = x % 6;
        if (d == 4 && prev == 3)
            return true;
        prev = d;
        x = x / 6;
    }
    return false;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t i = 27; ; ++i) {          /* 27 is the smallest number with a 43 pair */
        seq_t v = (seq_t)i;
        if (has43(v) && !has43(v - 1)) {
            ++count;
            if (count == n)
                return v;
        }
    }
}
