/*
 * A021613  Decimal expansion of 1/609.
 * https://oeis.org/A021613
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

/* Sequence: decimal expansion of 1/609 (period = 84) */
#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    enum { PERIOD = 84 };                      /* period length of the decimal expansion */
    static seq_t digits[PERIOD];               /* cached digits of 1/609 */
    static bool initialized = false;

    if (!initialized) {
        idx_t rem = 1;                         /* remainder of 1 ÷ 609 */
        for (size_t i = 0; i < PERIOD; ++i) {
            idx_t val = rem * 10;              /* bring down a zero */
            digits[i] = (seq_t)(val / 609);    /* next decimal digit */
            rem = val % 609;                   /* new remainder */
        }
        initialized = true;
    }
    size_t pos = (size_t)(n % PERIOD);        /* index into the period */
    return digits[pos];
}
