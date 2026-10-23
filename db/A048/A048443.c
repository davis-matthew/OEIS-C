/*
 * A048443  Take the first n numbers written in base 12, concatenate them,
 *          then convert from base 12 to base 10.
 * https://oeis.org/A048443
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = integer obtained by concatenating the base‑12
 *            representations of 1,…,n and interpreting the result as a
 *            base‑12 number
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 14, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    seq_t result = 0;
    for (idx_t k = 1; k <= n; ++k) {
        /* Extract digits of k in base 12, least significant first */
        uint8_t digits[64];
        size_t len = 0;
        idx_t temp = k;
        do {
            digits[len++] = (uint8_t)(temp % 12);
            temp /= 12;
        } while (temp);
        /* Append digits to the result, most significant first */
        for (size_t p = 0; p < len; ++p) {
            result = result * 12 + digits[len - 1 - p];
        }
    }
    return result;
}
