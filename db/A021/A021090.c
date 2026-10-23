/*
 * A021090  Decimal expansion of 1/86.
 * https://oeis.org/A021090
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   the nth digit after the decimal point in the decimal expansion
 *            of 1/86
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    uint64_t r = 1;           /* current remainder (always < 86) */
    seq_t digit = 0;

    /* Long‑division simulation: 1/86.
       For each step: r ← r * 10; digit ← r / 86; r ← r % 86.    */
    for (idx_t i = 0; i <= n; ++i) {
        uint64_t rtmp = r * 10ULL;  /* rtmp < 860, safe */
        digit = rtmp / 86ULL;
        r    = rtmp % 86ULL;
    }
    return digit;
}
