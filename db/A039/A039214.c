/*
 * A039214  Numbers whose base-12 representation has the same number of 0's
 *          and 5's.
 * https://oeis.org/A039214
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers whose base-12 representation has the same number of 0's and 5's.
   This function searches for the n-th such number by linear scanning. */
#include <stdbool.h>

/* Return true iff x has equal counts of 0 and 5 in its base‑12 representation. */
static bool equal_zero_five(seq_t x)
{
    int z = 0, f = 0;
    while (x > 0) {
        seq_t d = x % 12;
        if (d == 0) ++z;
        else if (d == 5) ++f;
        x = x / 12;
    }
    return z == f;
}

seq_t a(idx_t n)
{
    seq_t cand = 1;
    idx_t found = 0;

    while (1) {
        if (equal_zero_five(cand)) {
            ++found;
            if (found == n) return cand;
        }
        ++cand;
    }
}
