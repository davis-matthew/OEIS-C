/*
 * A095609  a(n) = 109 written in base 10 - n.
 * https://oeis.org/A095609
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = digits of 109 written in base (10 - n), read as a decimal
 *            number. For n ≥ 10 the sequence is undefined, so this function
 *            returns 0
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Series only defined for 0 ≤ n ≤ 9 */
    if (n > 9) return 0;

    /* Base for conversion: 10 - n (positive integer 1..10) */
    int base = 10 - (int)n;            /* base ∈ {1,…,10} */

    /* 109 in base 10 is the constant to convert. */
    int m = 109;
    /* For base 1 we need up to 109 digits. */
    int digits[110];
    int len = 0;

    if (base == 1) {
        /* Unary representation: all digits are 1 */
        len = 109;
        for (int i = 0; i < len; ++i) digits[i] = 1;
    } else {
        while (m > 0) {
            digits[len++] = m % base;
            m /= base;
        }
    }

    /* Build the decimal integer by reading digits most significant first */
    seq_t result = 0;
    for (int i = len - 1; i >= 0; --i)
        result = result * 10 + (seq_t)digits[i];

    return result;
}
