/*
 * A039032  Numbers whose base-7 representation has the same number of 0's and
 *          3's.
 * https://oeis.org/A039032
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 125, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 83 terms through n = 9702 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth natural number whose base‑7 representation has
   an equal number of 0's and 3's.  The function scans the natural numbers
   sequentially, counting such numbers until the n‑th is found. */
#include <stdbool.h>

static inline bool has_equal_zero_three(seq_t num)
{
    seq_t tmp = num;
    unsigned cnt0 = 0, cnt3 = 0;
    while (tmp > 0) {
        int d = (int)(tmp % 7);
        if (d == 0) ++cnt0;
        else if (d == 3) ++cnt3;
        tmp = tmp / 7;
    }
    return cnt0 == cnt3;
}

seq_t a(idx_t n)
{
    idx_t found = 0;
    for (seq_t val = 1; ; ++val) {
        if (has_equal_zero_three(val)) {
            ++found;
            if (found == n) return val;
        }
    }
}
