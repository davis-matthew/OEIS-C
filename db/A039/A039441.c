/*
 * A039441  Numbers whose base-11 representation has the same nonzero number
 *          of 0's and 4's.
 * https://oeis.org/A039441
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Validated: partial, pass, 46 terms through n = 46 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A039441: numbers whose base-11 representation has the same
 * nonzero number of 0's and 4's.
 */
#include <stdbool.h>

/* Check whether a number has equal non‑zero counts of 0 and 4 in base‑11. */
static bool equal_zero_four(seq_t x)
{
    unsigned cnt0 = 0, cnt4 = 0;
    while (x > (seq_t)0) {
        unsigned digit = (unsigned)(x % (seq_t)11);
        if (digit == 0) ++cnt0;
        else if (digit == 4) ++cnt4;
        x /= (seq_t)11;
    }
    return (cnt0 > 0) && (cnt0 == cnt4);
}

seq_t a(idx_t n)
{
    idx_t found = 0;
    seq_t cur = (seq_t)1;  /* start from 1, as 0 cannot satisfy the condition */

    while (1) {
        if (equal_zero_four(cur)) {
            ++found;
            if (found == n) return cur;
        }
        ++cur;  /* next integer */
    }
}
