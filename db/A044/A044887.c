/*
 * A044887  Numbers having, in base 16, (sum of even run lengths)=(sum of odd
 *          run lengths).
 * https://oeis.org/A044887
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth integer x >= 4097 such that the sum of the
 *            lengths of even runs of equal hexadecimal digits equals the sum
 *            of the lengths of odd runs, where runs are maximal consecutive
 *            equal digits in the hexadecimal representation of x
 * Range:     u8 none, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static inline bool balanced(seq_t x)
{
    /* Handles numbers x >= 1. Assumes x is non‑zero. */
    long long diff = 0;          /* even sum minus odd sum (positive or negative) */
    int run_len = 1;             /* length of current run */
    int prev_digit = (int)(x % 16);
    x /= 16;

    while (x > 0) {
        int d = (int)(x % 16);
        if (d == prev_digit)
            run_len++;
        else {
            if (run_len % 2 == 0)
                diff += run_len;
            else
                diff -= run_len;
            prev_digit = d;
            run_len = 1;
        }
        x /= 16;
    }

    /* last run */
    if (run_len % 2 == 0)
        diff += run_len;
    else
        diff -= run_len;
    return diff == 0;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t value = 4097;           /* first term in the sequence */

    while (1) {
        if (balanced(value)) {
            count++;
            if (count == n)
                return value;
        }
        value++;
    }
}
