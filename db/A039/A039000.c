/*
 * A039000  Numbers whose base-3 representation has the same number of 0's and
 *          1's.
 * https://oeis.org/A039000
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: nth number whose base‑3 representation has the
 * same number of 0's and 1's.  This implementation
 * simply scans integers in increasing order, counts the
 * 0‑digits and 1‑digits in base 3, and returns the
 * n‑th matching number.  It works for any seq_t
 * (uint8_t, uint16_t, …, unsigned __int128, or an
 * arbitrary‑precision integer class) and any idx_t
 * (uint64_t or larger).  The search stops as soon
 * as the required count is reached.  It is efficient
 * for the range n ≤ 10 000 (under 2 seconds). */

#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t count = 0;          /* number of valid numbers found */

    /* Scan natural numbers from 2 upwards.  The
     * sequence starts at 2 because 2 in base‑3 is "2"
     * (zero 0‑digits and zero 1‑digits). */
    for (uint64_t x = 2;; ++x) {
        uint64_t y   = x;
        unsigned cnt0 = 0, cnt1 = 0;

        /* Count 0‑digits and 1‑digits in base‑3. */
        while (y > 0) {
            int d = y % 3;
            if (d == 0) ++cnt0;
            else if (d == 1) ++cnt1;
            y /= 3;
        }

        if (cnt0 == cnt1) {
            ++count;
            if (count == n)
                return (seq_t)x;   /* cast to the appropriate seq_t type */
        }
    }
}
