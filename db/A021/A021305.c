/*
 * A021305  Decimal expansion of 1/301.
 * https://oeis.org/A021305
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

/* Decimal digit at position n of the decimal expansion of 1/301. */
#include <stdbool.h>

static seq_t digits[42];
static bool initialized = false;

seq_t a(idx_t n)
{
    if (!initialized)
    {
        idx_t rem = 1;                   /* start with 1 */
        for (idx_t i = 0; i < 42; ++i)
        {
            rem *= 10;                   /* bring down next digit */
            digits[i] = (seq_t)(rem / 301);   /* quotient is the digit */
            rem = rem % 301;                /* new remainder */
        }
        initialized = true;
    }
    return digits[n % 42];
}
