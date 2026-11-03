/*
 * A039487  Numbers whose base-11 representation has the same nonzero number
 *          of 7's and 8's.
 * https://oeis.org/A039487
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Validated: partial, pass, 48 terms through n = 48 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: numbers whose base-11 representation has the same nonzero
 * number of 7's and 8's.
 */

#include <stdbool.h>

/* Return true if v has equal nonzero numbers of digits 7 and 8 in base 11. */
static inline bool has_equal_7_8(seq_t v) {
    seq_t tmp = v;
    int cnt7 = 0, cnt8 = 0;
    while (tmp > 0) {
        int d = (int)(tmp % 11);
        if (d == 7)   cnt7++;
        else if (d == 8) cnt8++;
        tmp /= 11;
    }
    return cnt7 > 0 && cnt7 == cnt8;
}

seq_t a(idx_t n) {
    idx_t count = 0;
    idx_t cur   = 1;
    while (count < n) {
        if (has_equal_7_8((seq_t)cur)) {
            count++;
            if (count == n)
                return (seq_t)cur;
        }
        cur++;
    }
    /* Unreachable for valid n>=1 */
    return (seq_t)0;
}
