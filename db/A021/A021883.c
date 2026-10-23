/*
 * A021883  Decimal expansion of 1/879.
 * https://oeis.org/A021883
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = nth digit of the decimal expansion of 1/879 after the
 *            decimal point
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Precompute digits of the repeating decimal 1/879.
       The period is at most 584 (φ(879)).  We'll store it in a static array. */
    static seq_t digits[600];
    static size_t period = 0;
    static int initialized = 0;

    if (!initialized) {
        unsigned int rem = 1;          /* initial remainder 1 % 879 */
        size_t i = 0;
        do {
            seq_t digit = (seq_t)(rem * 10) / 879;
            digits[i++] = digit;
            rem = (rem * 10) % 879;
        } while (rem != 1);
        period = i;
        initialized = 1;
    }

    size_t pos = (size_t)(n % period);
    return digits[pos];
}
