/*
 * A126206  Number of 4's in the decimal expansion of 4^n.
 * https://oeis.org/A126206
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of decimal digit 4 in 4^n
 * Range:     u8 n <= 3744, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Maximum number of decimal digits for 4^n with n ≤ 10000 is below 7000 */
    const size_t MAX_DIG = 7000;
    uint8_t digits[MAX_DIG];

    size_t len = 1;          /* current number of digits (little‑endian) */
    digits[0] = 1;           /* 4^0 = 1 */

    for (idx_t i = 0; i < n; ++i) {
        uint32_t carry = 0;
        for (size_t j = 0; j < len; ++j) {
            uint32_t prod = (uint32_t)digits[j] * 4u + carry;
            digits[j] = (uint8_t)(prod % 10u);
            carry    = prod / 10u;
        }
        while (carry != 0u) {
            digits[len++] = (uint8_t)(carry % 10u);
            carry /= 10u;
        }
    }

    seq_t count4 = 0;
    for (size_t j = 0; j < len; ++j) {
        if (digits[j] == 4u) {
            ++count4;
        }
    }
    return count4;
}
