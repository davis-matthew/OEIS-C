/*
 * A065716  Number of 5's in decimal expansion of 2^n.
 * https://oeis.org/A065716
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   number of digit 5's in decimal expansion of 2^n
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Store the decimal number 2^k in little‑endian digit arrays.
       digits[0] holds the least significant decimal digit.  */
    uint8_t digits[4000] = {0};
    size_t len = 1;        /* current number of digits (at least 1)      */
    digits[0] = 1;         /* 2^0 = 1  */

    /* Multiply by 2 exactly n times. */
    for (idx_t i = 0; i < n; ++i) {
        unsigned char carry = 0;
        for (size_t j = 0; j < len; ++j) {
            unsigned int t = digits[j] * 2 + carry;
            digits[j] = t % 10;
            carry    = t / 10;
        }
        while (carry) {
            digits[len++] = carry % 10;
            carry        /= 10;
        }
    }

    /* Count the digit '5' in the decimal representation. */
    seq_t count = 0;
    for (size_t i = 0; i < len; ++i) {
        if (digits[i] == 5) {
            count = count + 1;
        }
    }
    return count;
}
