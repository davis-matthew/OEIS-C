/*
 * A039100  Numbers whose base-9 representation has the same number of 3's and
 *          7's.
 * https://oeis.org/A039100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 154, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 85 terms through n = 8981 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool equal_37_base9(seq_t x)
{
    int cnt3 = 0, cnt7 = 0;
    const seq_t nine = 9;            /* constant 9 converted to seq_t */
    while (x != 0) {
        seq_t rem = x % nine;
        if (rem == 3) ++cnt3;
        else if (rem == 7) ++cnt7;
        x /= nine;
    }
    return cnt3 == cnt7;
}

/* Search: a(n) is the nth integer whose base‑9 representation
   contains the same number of 3’s and 7’s.  Implements OEIS A039100. */

seq_t a(idx_t n)
{
    seq_t cur = 0;
    idx_t found = 0;
    const seq_t one = 1;
    while (true) {
        if (equal_37_base9(cur)) {
            ++found;
            if (found == n)
                return cur;
        }
        cur += one;
    }
}
