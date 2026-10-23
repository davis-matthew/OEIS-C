/*
 * A103829  Sum of even digits greater than sum of odd digits.
 * https://oeis.org/A103829
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 111, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   even_gt_odd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the nth positive integer whose sum of even decimal digits exceeds the sum of odd decimal digits. */

/* Compute up to the required term.  The algorithm scans
 * successive integers, counting those that satisfy the
 * inequality.  For n up to about 10 000 this typically
 * examines no more than twice as many integers. */
#include <stdbool.h>
#include "../utils/even_gt_odd.h"

seq_t a(idx_t n)
{
    if (n == 0)    /* undefined, but safe fallback */
        return 0;
    idx_t      count = 0;
    seq_t      val   = 1;   /* start from 1 (0 is not included) */

    for (;;) {
        if (even_gt_odd(val)) {
            ++count;
            if (count == n)
                return val;
        }
        ++val;
    }
}
