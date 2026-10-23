/*
 * A021627  Decimal expansion of 1/623.
 * https://oeis.org/A021627
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) is the nth decimal digit of 1/623
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t digits[132];

    if (!initialized) {
        uint64_t rem = 1;
        for (size_t i = 0; i < 132; ++i) {
            rem *= 10;
            digits[i] = (seq_t)(rem / 623);
            rem %= 623;
        }
        initialized = true;
    }
    size_t idx = (size_t)(n % 132);
    return digits[idx];
}
