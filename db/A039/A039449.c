/*
 * A039449  Numbers whose base-11 representation has the same nonzero number
 *          of 1's and 3's.
 * https://oeis.org/A039449
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = the n-th positive integer whose base‑11 representation
 * has an equal (non‑zero) number of digits 1 and 3.
 *
 * The function scans successive integers, testing each for the property:
 * number of '1' digits == number of '3' digits and at least one of each.
 * It returns the nth such number.  This works for any n ≥ 1.
 *
 * No external static state is used – each call recomputes the sequence
 * from scratch.  The algorithm is O(n · number-of-digits), which is
 * well below the 2‑second limit for n up to 10 000.
 */

#include <stdbool.h>
#include <stdint.h>

/* Helper: test whether x satisfies the property. */
static bool matches(seq_t x)
{
    /* Count 1's and 3's in base‑11 representation. */
    int cnt1 = 0, cnt3 = 0;
    seq_t y = x;
    while (y > 0) {
        /* y % 11 fits in a native integer. */
        uint64_t digit = (uint64_t)(y % (seq_t)11);
        if (digit == 1) ++cnt1;
        else if (digit == 3) ++cnt3;
        y = y / (seq_t)11;
    }
    return (cnt1 == cnt3) && (cnt1 > 0);
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    idx_t found = 0;
    seq_t num = 1;                  /* start from 1, will hit 14 first */
    while (true) {
        if (matches(num)) {
            ++found;
            if (found == n) {
                return num;
            }
        }
        ++num;                      /* next candidate */
    }
}
