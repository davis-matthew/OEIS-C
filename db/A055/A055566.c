/*
 * A055566  Sum of digits of n^5.
 * https://oeis.org/A055566
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = sum of decimal digits of n^5
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* Special case n = 0 */
    if (n == 0)
        return 0;

    /* Base value to be used for multiplication */
    uint64_t base = (uint64_t)n;

    /* Array to hold decimal digits of the current power (least‑significant first) */
    uint8_t digits[200];
    size_t len = 0;

    /* Initialise with n */
    uint64_t temp = base;
    while (temp > 0) {
        digits[len++] = (uint8_t)(temp % 10);
        temp /= 10;
    }
    if (len == 0) {          /* n was 0, but we already returned */
        digits[len++] = 0;
    }

    /* Multiply by n four more times to obtain n^5 */
    for (int mult = 1; mult < 5; ++mult) {
        unsigned __int128 carry = 0;
        for (size_t i = 0; i < len; ++i) {
            unsigned __int128 prod = (unsigned __int128)digits[i] * base + carry;
            digits[i] = (uint8_t)(prod % 10);
            carry = prod / 10;
        }
        while (carry) {
            digits[len++] = (uint8_t)(carry % 10);
            carry /= 10;
        }
    }

    /* Sum the digits (use a wider type to avoid temporary overflow) */
    unsigned long long sum = 0;
    for (size_t i = 0; i < len; ++i)
        sum += digits[i];

    return (seq_t)sum;
}
