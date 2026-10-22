/*
 * A000493  a(n) = floor(sin(n)).
 * https://oeis.org/A000493
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -(floor(n/Pi) mod 2), since sin(n) < 0 exactly when
 *            floor(n/Pi) is odd (floor(n/Pi) is computed exactly with a
 *            128-bit fixed-point value of 1/Pi)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* 1/Pi = 0x0.517CC1B727220A94FE13ABE8FA9A6EE0... */
    const uint64_t hi = 0x517CC1B727220A94ULL;
    const uint64_t lo = 0xFE13ABE8FA9A6EE0ULL;
    unsigned __int128 t = (unsigned __int128)n * lo;
    unsigned __int128 s = (unsigned __int128)n * hi + (t >> 64);
    uint64_t q = (uint64_t)(s >> 64);
    seq_t r = 0;
    if (q & 1) r = r - 1;
    return r;
}
