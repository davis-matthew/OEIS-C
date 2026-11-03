/*
 * A103829  Sum of even digits greater than sum of odd digits.
 * https://oeis.org/A103829
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 111, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 79 terms through n = 9485 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the nth positive integer whose sum of even decimal digits exceeds the sum of odd decimal digits. */

/* Compute up to the required term.  The algorithm scans
 * successive integers, counting those that satisfy the
 * inequality.  For n up to about 10 000 this typically
 * examines no more than twice as many integers. */
#include <stdbool.h>

static bool even_gt_odd(seq_t x)
{
    seq_t sum_even = 0;
    seq_t sum_odd  = 0;
    for (seq_t t = x; t > 0; t /= 10) {
        seq_t d = t % 10;
        /* Zero is treated as even. */
        if ((d & 1) == 0)
            sum_even += d;
        else
            sum_odd += d;
    }
    return sum_even > sum_odd;
}

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
