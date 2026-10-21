/*
 * A224238  Decimal expansion of speed of light in miles per hour.
 * https://oeis.org/A224238
 *
 * Offset:    9    (a(9) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = decimal digit of the speed of light in miles per hour
 *            (offset 9: a(9) is the first integer digit 6 of
 *            670616629.384...)
 * Range:     u8 n <= 17, u16 n <= 17, u32 n <= 17, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* For indices below the offset the behaviour is irrelevant */
    if (n < 9) return 0;

    /* Position in the concatenated integer+fraction digits (0‑based) */
    idx_t pos = n - 9;

    /* The integer part has exactly 9 digits: 6 7 0 6 1 6 6 2 9 */
    if (pos < 9) {
        static const uint8_t int_digits[9] = {6,7,0,6,1,6,6,2,9};
        return (seq_t)int_digits[pos];
    }

    /* Fractional part: compute the digit at fractional position (pos-9) */
    idx_t fpos = pos - 9;

    /* Exact rational formula:
       speed_mph = 299792458 * 3600 * 1000 / 1609344
       Note: numerator = 1079252848800000, denominator = 1609344 */
    const seq_t N = 1079252848800000ULL;      /* 299792458 * 3600 * 1000 */
    const seq_t D = 1609344ULL;              /* 5280 * 0.3048 * 1000 */

    seq_t rem = N % D;   /* initial remainder */

    /* Generate digits one by one until the desired position is reached. */
    for (idx_t i = 0; i <= fpos; ++i) {
        rem *= 10;
        seq_t digit = rem / D;
        rem  %= D;
        if (i == fpos) return digit;
    }

    /* Unreachable return to satisfy the compiler */
    return 0;
}
