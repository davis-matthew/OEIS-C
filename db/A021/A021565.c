/*
 * A021565  Decimal expansion of 1/561.
 * https://oeis.org/A021565
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

/* Computes the nth digit after the decimal point of 1/561.
   The decimal period is 16 digits; the digits are produced by long division.
   a(n) = digit n (0‑indexed) after the decimal point. */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* digits of the repeating cycle of 1/561 (period = 16) */
    static seq_t period[16];
    static idx_t period_len = 0;
    static bool initialized = false;

    if (!initialized) {
        initialized = true;
        uint64_t rem = 1;          /* remainder starts as numerator 1 */
        idx_t i = 0;
        do {
            rem *= 10;                 /* bring next digit into the dividend */
            period[i] = (seq_t)(rem / 561); /* quotient is the next digit */
            rem %= 561;                /* new remainder */
            i++;
        } while (rem != 1);            /* stop when the remainder repeats */
        period_len = i;
    }

    return period[(size_t)(n % period_len)];
}
