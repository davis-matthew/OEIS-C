/*
 * A028909  Arrange digits of 2^n in ascending order.
 * https://oeis.org/A028909
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = arrangement of the decimal digits of 2^n in ascending
 *            order, leading zeros discarded
 * Range:     u8 n <= 7, u16 n <= 17, u32 n <= 33, u64 n <= 66, u128 n <= 139,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n) {
    /* Maximum number of decimal digits of 2^n for n <= 10000 is about 3011.
       Use a slightly larger static buffer for safety. */
    static const size_t MAX_DIGITS = 4000;
    uint8_t digits[MAX_DIGITS];
    size_t len = 1;
    digits[0] = 1;  /* initialize to 2^0 */

    /* Compute 2^n by repeated doubling. */
    for (idx_t i = 0; i < n; ++i) {
        uint8_t carry = 0;
        for (size_t j = 0; j < len; ++j) {
            uint8_t val = digits[j] * 2 + carry;
            digits[j] = val % 10;
            carry = val / 10;
        }
        while (carry > 0) {
            digits[len++] = carry % 10;
            carry /= 10;
        }
    }

    /* Count frequencies of each decimal digit. */
    uint32_t freq[10] = {0};
    for (size_t i = 0; i < len; ++i) {
        freq[digits[i]]++;
    }

    /* Reconstruct the number from digits 1..9 in ascending order
       (digits 0 are discarded as leading zeros). */
    seq_t result = 0;
    for (uint8_t d = 1; d <= 9; ++d) {
        uint32_t cnt = freq[d];
        for (uint32_t k = 0; k < cnt; ++k) {
            result = result * 10 + (seq_t)d;
        }
    }
    return result;
}
