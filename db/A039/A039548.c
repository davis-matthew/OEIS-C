/*
 * A039548  Numbers whose base-12 representation has the same nonzero number
 *          of 6's and 11's.
 * https://oeis.org/A039548
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n-th positive integer whose base-12 representation
 *            contains the same nonzero number of 6's and 11's
 * Range:     u8 n <= 3, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 45 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool has_equal_nonzero_six_and_eleven(idx_t x) {
    unsigned cnt6 = 0, cnt11 = 0;
    if (x == 0) return false;
    while (x > 0) {
        unsigned d = (unsigned)(x % 12);
        if (d == 6)      cnt6++;
        else if (d == 11)cnt11++;
        x /= 12;
    }
    return (cnt6 == cnt11) && (cnt6 > 0);
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;           /* undefined for n==0, just return 0 */
    idx_t found = 0;
    idx_t candidate = 1;
    while (found < n) {
        if (has_equal_nonzero_six_and_eleven(candidate))
            ++found;
        ++candidate;
    }
    return (seq_t)(candidate - 1);
}
