/*
 * A021205  Decimal expansion of 1/201.
 * https://oeis.org/A021205
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Decimal digit of 1/201 at position n (0‑indexed).  Cycle length = 33. */
#include <stddef.h>

seq_t a(idx_t n) {
    static const seq_t period[33] = {
        0, 0, 4, 9, 7, 5, 1, 2, 4, 3, 7, 8, 1, 0, 9,
        4, 5, 2, 7, 3, 6, 3, 1, 8, 4, 0, 7, 9, 6, 0,
        1, 9, 9
    };
    return period[n % 33];
}
