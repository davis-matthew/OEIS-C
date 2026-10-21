/*
 * A214024  Decimal expansion of 4^4^4.
 * https://oeis.org/A214024
 *
 * Offset:    155    (a(155) is the first term)
 * Keywords:  nonn, cons, fini, full, easy
 * Formula:   a(n) is the nth decimal digit of 2^512 (where index 155 is the
 *            most significant digit)
 * Range:     u8 n <= 309, u16 n <= 309, u32 n <= 309, u64 n <= 309,
 *            u128 n <= 309, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Precompute the decimal digits of 2^512 once. */
    static uint8_t digits[155];
    static size_t len = 0;

    if (len == 0) {
        digits[0] = 1;
        len = 1;
        for (int e = 0; e < 512; ++e) {
            uint16_t carry = 0;
            for (size_t i = 0; i < len; ++i) {
                uint16_t val = (uint16_t)digits[i] * 2 + carry;
                digits[i] = (uint8_t)(val % 10);
                carry     = val / 10;
            }
            while (carry) {
                digits[len++] = (uint8_t)(carry % 10);
                carry /= 10;
            }
        }
    }

    if (n < 155) return 0;          /* indices before the offset are undefined */
    size_t pos = (size_t)(n - 155); /* 0‑based from the most significant digit */
    if (pos >= len) return 0;        /* out‑of‑bounds digit: return 0 */

    return (seq_t)digits[len - 1 - pos];
}
