/*
 * A021875  Decimal expansion of 1/871.
 * https://oeis.org/A021875
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor(10^(n+1) / 871) % 10 (digit of the decimal
 *            expansion of 1/871)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t digits[871];
    static idx_t period = 0;

    if (period == 0) {   /* compute the repeating cycle once */
        int remainder = 1;
        idx_t i = 0;

        do {
            int digit = (remainder * 10) / 871;
            digits[i] = (seq_t)digit;
            remainder = (remainder * 10) % 871;
            ++i;
        } while (remainder != 1 && i < 871);

        period = i;   /* period is at most 870 */
    }

    return digits[n % period];
}
