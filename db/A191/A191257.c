/*
 * A191257  a(n) = A067368(n)/2.
 * https://oeis.org/A191257
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth positive integer whose 2-adic valuation is a
 *            multiple of 3
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t val   = 0;          /* start before first candidate */
    while (count < n) {
        val++;                /* next candidate */
        idx_t t = val;
        idx_t tz = 0;
        /* count trailing zeros (valuation of 2) */
        while ((t & 1) == 0) {
            t >>= 1;
            tz++;
        }
        if ((tz % 3) == 0) {
            count++;
            if (count == n)
                return (seq_t)val;
        }
    }
    /* unreachable, but return 0 to satisfy compiler */
    return (seq_t)0;
}
