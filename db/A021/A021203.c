/*
 * A021203  Decimal expansion of 1/199.
 * https://oeis.org/A021203
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = the n-th digit after the decimal point in the decimal
 *            expansion of 1/199. The expansion is purely repeating with
 *            period 198
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Precompute the 198-periodic decimal digits once. */
    static bool initialized = false;
    static uint8_t digits[198];

    if (!initialized) {
        uint64_t rem = 1;                     /* 1/199 starts with remainder 1 */
        for (size_t i = 0; i < 198; ++i) {
            rem *= 10;
            digits[i] = (uint8_t)(rem / 199);
            rem     %= 199;
        }
        initialized = true;
    }

    size_t idx = (size_t)(n % 198);          /* index into the period */
    return (seq_t)digits[idx];
}
