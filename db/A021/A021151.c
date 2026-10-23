/*
 * A021151  Decimal expansion of 1/147.
 * https://oeis.org/A021151
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Decimal expansion of 1 ÷ 147.
   a(n) is the nth digit after the decimal point, periodic with period 42. */

#include <stddef.h>
#include <stdbool.h>

#define PERIOD 42

seq_t a(idx_t n)
{
    static seq_t digits[PERIOD];
    static bool initialized = false;

    if (!initialized) {
        unsigned long long rem = 1ULL;          /* 1/147 starts with remainder 1 */
        for (size_t i = 0; i < PERIOD; ++i) {
            rem *= 10ULL;                      /* bring next decimal digit */
            digits[i] = (seq_t)(rem / 147ULL); /* digit = floor(rem/147) */
            rem %= 147ULL;                     /* new remainder */
        }
        initialized = true;
    }

    return digits[(size_t)(n % PERIOD)];
}
