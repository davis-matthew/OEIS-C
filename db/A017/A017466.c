/*
 * A017466  a(n) = (11*n + 6)^6.
 * https://oeis.org/A017466
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11*n + 6)^6
 * Range:     u8 none, u16 n <= 0, u32 n <= 3, u64 n <= 147, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Compute the base (11*n + 6) in seq_t */
    seq_t base = (seq_t)11 * n + 6;

    /* Raise base to the 6th power by successive multiplication */
    seq_t result = base * base * base * base * base * base;

    return result;
}
