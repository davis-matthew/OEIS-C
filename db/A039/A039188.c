/*
 * A039188  Numbers whose base-11 representation has the same number of 3's
 *          and 10's.
 * https://oeis.org/A039188
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Validated: partial, pass, 70 terms through n = 70 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A039188: numbers whose base-11 representation has the same number of
 * 3's and 10's (digit value 10).  The function returns the nth such number
 * (n >= 1).  The computation is performed by a simple scan, checking each
 * non‑negative integer until the nth match is found. */

#include <stdbool.h>

static bool good_base11(seq_t x)
{
    int c3 = 0, c10 = 0;
    seq_t t = x;
    /* Handle the special case x == 0 explicitly to avoid the do‑while
     * loop being skipped when x == 0. */
    if (t == 0) {
        return true; /* 0 has zero 3's and zero 10's. */
    }
    while (t > 0) {
        int d = (int)(t % 11); /* digit in base 11 */
        if (d == 3)          ++c3;
        else if (d == 10)    ++c10;
        t /= 11;
    }
    return c3 == c10;
}

seq_t a(idx_t n)
{
    idx_t found = 0;
    seq_t val   = 0;

    while (found < n) {
        if (good_base11(val))
            ++found;
        if (found == n)
            return val;
        val++; /* increment for next candidate */
    }
    /* This point will never be reached because the loop exits when found==n. */
    return val;
}
