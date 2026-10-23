/*
 * A021751  Decimal expansion of 1/747.
 * https://oeis.org/A021751
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Decimal digits of 1/747 */
#include <stdbool.h>

seq_t a(idx_t n)
{
    static bool init = false;
    static idx_t period_len = 0;
    static seq_t digits[500];

    if (!init) {
        idx_t rem = 1;
        idx_t count = 0;
        do {
            rem *= 10;
            seq_t digit = (seq_t)(rem / 747);
            rem %= 747;
            digits[count++] = digit;
        } while (rem != 1);              /* cycle finished */
        period_len = count;
        init = true;
    }

    idx_t r = n % period_len;
    return digits[r];
}
