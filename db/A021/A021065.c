/*
 * A021065  Decimal expansion of 1/61.
 * https://oeis.org/A021065
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) is the nth decimal digit (after the point) of 1/61. The
 *            decimal expansion of 1/61 has period 60, so the sequence repeats
 *            every 60 digits
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t digits[60];
    static bool init = false;
    if (!init) {
        seq_t rem = 1;
        for (idx_t i = 0; i < 60; ++i) {
            rem = rem * 10;
            seq_t d = rem / 61;
            digits[i] = d;
            rem = rem % 61;
        }
        init = true;
    }
    return digits[n % 60];
}
