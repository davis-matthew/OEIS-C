/*
 * A039691  If n=x1x2...xm in base 10, n belongs to the sequence iff
 *          x1x2..xm*11=y1y2...ym and xm..x2x1*11=ym...y2y1.
 * https://oeis.org/A039691
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 154, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = n-th non‑negative integer with no two adjacent decimal digits summing to more than 9. */
#include <stdint.h>
#include <stdbool.h>

static bool good_uint64(uint64_t x)
{
    uint64_t prev = x % 10;
    x /= 10;
    while (x > 0) {
        uint64_t cur = x % 10;
        if (prev + cur > 9) return false;
        prev = cur;
        x /= 10;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* n >= 1 by sequence offset */
    idx_t found = 0;
    for (uint64_t val = 0; ; ++val) {
        if (good_uint64(val)) {
            ++found;
            if (found == n)
                return (seq_t)val;
        }
    }
}
