/*
 * A021493  Decimal expansion of 1/489.
 * https://oeis.org/A021493
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

/* a(n) = the nth digit after the decimal point in 1/489
   Formula: a(n) = floor(10^(n+1)/489) mod 10
*/
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *digits = NULL;
    static idx_t period = 0;

    /* Compute the repeating block once */
    if (digits == NULL) {
        const idx_t divisor = 489;
        /* Find the period length */
        idx_t r = 10 % divisor;
        period = 1;
        while (r != 1) {
            r = (r * 10) % divisor;
            period++;
        }

        /* Allocate storage for the digits */
        digits = (seq_t *)malloc(period * sizeof(*digits));

        /* Generate the digits of one period */
        idx_t rem = 1;
        for (idx_t i = 0; i < period; ++i) {
            rem *= 10;
            digits[i] = (seq_t)(rem / divisor);   /* 0 ≤ digit ≤ 9 */
            rem %= divisor;
        }
    }

    return digits[n % period];
}
