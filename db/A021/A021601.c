/*
 * A021601  Decimal expansion of 1/597.
 * https://oeis.org/A021601
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = the nth digit after the decimal point in the decimal *
 *            expansion of 1/597. The period of this expansion is 99, so the
 *            digits * repeat every 99 positions
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t digits[99];
    static bool initialized = false;

    if (!initialized) {
        uint64_t rem = 1;
        const uint64_t denom = 597;
        for (idx_t i = 0; i < (idx_t)99; ++i) {
            rem *= 10ull;          /* scaling by 10 */
            digits[i] = (seq_t)(rem / denom); /* quotient → digit */
            rem %= denom;          /* new remainder */
        }
        initialized = true;
    }

    return digits[(size_t)(n % (idx_t)99)];
}
