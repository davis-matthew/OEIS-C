/*
 * A134724  Successive decimal digits of the cubes A000578.
 * https://oeis.org/A134724
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = the n-th decimal digit in the concatenation of all
 *            non‑negative * cubes starting from 0^3, 1^3, 2^3, … (the digits
 *            of “0 1 8 27 64 125 …”). * * The function iterates through
 *            cubes, subtracts their digit counts * until the desired index
 *            falls within the current cube, then extracts * the cor...
 * Range:     u8 n <= 12, u16 n <= 923, u32 n <= 8000, u64 n <= 8000,
 *            u128 n <= 8000, big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"
#include "../utils/digit_len.h"

seq_t a(idx_t n) {
    idx_t remaining = n;                /* index to locate within the stream */
    for (idx_t k = 0;; ++k) {
        seq_t cube = (seq_t)k * k * k;  /* compute k^3 in seq_t */
        idx_t digits = digit_len(cube); /* number of decimal digits of cube */

        if (remaining < digits) {
            /* Position within this cube is 'remaining'.
             * Extract that digit efficiently using a power of ten. */
            idx_t shift = digits - 1 - remaining;
            seq_t divisor = pow10(shift);
            seq_t digit   = (cube / divisor) % 10;
            return digit;
        }

        remaining -= digits;            /* move past this cube's digits */
    }
}
