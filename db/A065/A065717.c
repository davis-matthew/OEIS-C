/*
 * A065717  Number of 6's in decimal expansion of 2^n.
 * https://oeis.org/A065717
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of digit 6 in the decimal expansion of 2^n
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Maximum number of decimal digits needed for 2^10000. 2^10000 ≈ 10^3010.3 */
#define MAX_DIGITS 4000

seq_t a(idx_t n) {
    /* Use a static array of decimal digits (little‑endian) */
    static uint8_t digits[MAX_DIGITS];
    size_t len = 1;          /* current number of digits */
    digits[0] = 1;           /* 2^0 = 1 */

    for (idx_t exp = 0; exp < n; ++exp) {
        unsigned int carry = 0;
        for (size_t i = 0; i < len; ++i) {
            unsigned int tmp = (unsigned int)digits[i] * 2 + carry;
            digits[i] = (uint8_t)(tmp % 10);
            carry = tmp / 10;
        }
        if (carry) {
            digits[len++] = (uint8_t)carry;
        }
    }

    seq_t count = 0;
    for (size_t i = 0; i < len; ++i) {
        if (digits[i] == 6) {
            ++count;
        }
    }
    return count;
}
