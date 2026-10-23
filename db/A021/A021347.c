/*
 * A021347  Decimal expansion of 1/343.
 * https://oeis.org/A021347
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10^(n+1) / 343) mod 10 (decimal digit n after the
 *            point of 1/343)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* 1/343 has period 294 in decimal expansion. */
    #define PERIOD 294
    static seq_t digits[PERIOD];
    static int initialized = 0;

    if (!initialized) {
        idx_t rem = 1;   /* remainder for division */
        for (idx_t i = 0; i < PERIOD; ++i) {
            digits[i] = (seq_t)((rem * 10) / 343);
            rem = (rem * 10) % 343;
        }
        initialized = 1;
    }

    return digits[n % PERIOD];
}
