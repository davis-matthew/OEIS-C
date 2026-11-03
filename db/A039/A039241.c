/*
 * A039241  Numbers whose base-12 representation has the same number of 3's
 *          and 6's.
 * https://oeis.org/A039241
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   numbers whose base-12 representation has the same number of 3's
 *            and 6's
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Validated: partial, pass, 69 terms through n = 69 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool has_equal_3_and_6(uint64_t x)
{
    unsigned int cnt3 = 0, cnt6 = 0;
    if (x == 0) return true;  /* 0 has 0 3's and 0 6's */
    while (x != 0) {
        unsigned int d = x % 12;
        if (d == 3) ++cnt3;
        else if (d == 6) ++cnt6;
        x /= 12;
    }
    return cnt3 == cnt6;
}

seq_t a(idx_t n)
{
    idx_t pos = 0;          /* zero‑based counter of qualifying numbers */
    idx_t cur = 0;          /* current integer being examined */
    while (1) {
        if (has_equal_3_and_6(cur)) {
            ++pos;
            if (pos == n) return (seq_t)cur;
        }
        ++cur;
    }
}
