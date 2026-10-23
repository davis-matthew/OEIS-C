/*
 * A037436  Positive numbers having the same set of digits in base 6 and base
 *          9.
 * https://oeis.org/A037436
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 288, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A037436: numbers having the same set of digits in base 6 and base 9
   The function scans positive integers and returns the nth one that satisfies
   the property. */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t count = 0;

    for (idx_t val = 1; ; ++val) {
        /* mask of digits appearing in base‑6 representation */
        uint8_t mask6 = 0;
        idx_t t = val;
        while (t) {
            mask6 |= (uint8_t)1 << (t % 6);
            t /= 6;
        }

        /* mask of digits appearing in base‑9 representation,
           check that no digit ≥6 occurs */
        uint8_t mask9 = 0;
        int ok = 1;
        t = val;
        while (t) {
            int d = t % 9;
            if (d >= 6) {   /* digits 6‑8 not allowed */
                ok = 0;
                break;
            }
            mask9 |= (uint8_t)1 << d;
            t /= 9;
        }
        if (!ok)
            continue;

        if (mask6 == mask9) {
            ++count;
            if (count == n)
                return (seq_t)val;
        }
    }
}
