/*
 * A039371  Numbers whose base-9 representation has the same nonzero number of
 *          1's and 8's.
 * https://oeis.org/A039371
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 8513, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 69 terms through n = 9965 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A039371: numbers whose base-9 representation has the same
   non‑zero number of 1's and 8's.  The function computes the nth term
   (n >= 1) by a simple enumeration. */

/*----------------------------------------------------------------------------*/

#include <stdbool.h>

/* Helper: test if a number has the same count of 1 and 8 in base‑9
   (and both counts > 0). */
static inline bool has_equal_1_and_8(seq_t v)
{
    seq_t tmp = v;
    unsigned cnt1 = 0, cnt8 = 0;
    /* Greedy conversion to base‑9 */
    while (tmp > 0) {
        int d = (int)(tmp % 9);
        if (d == 1) ++cnt1;
        else if (d == 8) ++cnt8;
        tmp /= 9;
    }
    return cnt1 == cnt8 && cnt1 > 0;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    seq_t count = 0;
    seq_t x = 1;

    /* Enumerate integers until we have found the nth valid one */
    while (count < n) {
        if (has_equal_1_and_8(x)) {
            ++count;
            if (count == n)
                return x;
        }
        ++x;
    }
    /* Unreachable, but satisfies compiler */
    return 0;
}
