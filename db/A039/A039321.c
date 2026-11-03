/*
 * A039321  Numbers whose base-7 representation has the same nonzero number of
 *          2's and 5's.
 * https://oeis.org/A039321
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Validated: partial, pass, 52 terms through n = 52 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers whose base-7 representation has equal nonzero counts of 2 and 5 */
#include <stdlib.h>

static int equal_2_and_5_nonzero(seq_t x) {
    int cnt2 = 0, cnt5 = 0;
    seq_t t = x;
    while (t > 0) {
        int d = (int)(t % 7);
        if (d == 2) ++cnt2;
        else if (d == 5) ++cnt5;
        t /= 7;
    }
    return cnt2 > 0 && cnt2 == cnt5;
}

seq_t a(idx_t n) {
    idx_t found = 0;
    seq_t candidate = 1;
    while (1) {
        if (equal_2_and_5_nonzero(candidate)) {
            ++found;
            if (found == n) return candidate;
        }
        ++candidate;
    }
}
