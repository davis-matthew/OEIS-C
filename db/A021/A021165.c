/*
 * A021165  Decimal expansion of 1/161.
 * https://oeis.org/A021165
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

/* Sequence: a(n) = nth decimal digit of 1/161, periodic with period 66 */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Store the 66 digits of the decimal expansion of 1/161.
       The digits are computed lazily only on the first call. */
    static seq_t digits[66];
    static bool initialized = false;

    if (!initialized) {
        idx_t remainder = 1;               /* start with 1 / 161 */
        for (size_t i = 0; i < 66; ++i) {
            /* Compute the next digit: floor(10 * remainder / 161) */
            digits[i] = (seq_t)((remainder * 10) / 161);
            /* Update the remainder: (10 * remainder) mod 161 */
            remainder = (remainder * 10) % 161;
        }
        initialized = true;
    }

    /* The sequence is periodic with period 66. */
    return digits[(size_t)(n % 66)];
}
