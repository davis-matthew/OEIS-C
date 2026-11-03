/*
 * A039259  Numbers whose base-12 representation has the same number of 5's
 *          and 11's.
 * https://oeis.org/A039259
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

/* Property: numbers whose base‑12 representation has equal count of 5's and 11's. */

#include <stdbool.h>

/* Return true iff v has the same number of digit 5 and digit 11 in base‑12. */
static bool has_equal5_and11(seq_t v) {
    unsigned int cnt5 = 0, cnt11 = 0;
    while (v > 0) {
        seq_t rem = v % 12;
        unsigned long long d = (unsigned long long)rem;
        if (d == 5) ++cnt5;
        else if (d == 11) ++cnt11;
        v /= 12;
    }
    return cnt5 == cnt11;
}

seq_t a(idx_t n) {
    seq_t candidate = 0;
    idx_t found = 0;
    while (1) {
        if (has_equal5_and11(candidate)) {
            ++found;
            if (found == n) return candidate;
        }
        ++candidate;
    }
}
