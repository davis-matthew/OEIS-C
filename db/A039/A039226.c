/*
 * A039226  Numbers whose base-12 representation has the same number of 1's
 *          and 8's.
 * https://oeis.org/A039226
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   numbers whose base-12 representation has the same number of 1's
 *            and 8's
 * Range:     u8 n <= 119, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool good(seq_t x)
{
    unsigned ones = 0, eights = 0;
    while (x) {
        unsigned d = (unsigned)(x % 12);
        if (d == 1) ++ones;
        else if (d == 8) ++eights;
        x /= 12;
    }
    return ones == eights;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset is 1, but guard against 0 */
    idx_t cnt = 0;
    seq_t val = 0;
    while (1) {
        if (good(val)) {
            ++cnt;
            if (cnt == n) return val;
        }
        ++val;
    }
}
