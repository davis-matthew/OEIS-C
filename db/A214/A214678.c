/*
 * A214678  a(n) = n represented in bijective base-8 numeration.
 * https://oeis.org/A214678
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n represented in bijective base‑8 numeration, interpreted
 *            as a decimal integer
 * Range:     u8 n <= 173, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* For indices below the offset return 0. */
    if (n == 0) return 0;

    /* Temporary array large enough for 64‑bit numbers in base‑8 (max 21 digits). */
    uint8_t digits[64];
    size_t len = 0;

    /* Convert n to bijective base‑8 digits (least significant first). */
    while (n > 0) {
        uint64_t r = n % 8;           /* r in [0,7] */
        if (r == 0) {                 /* bijective adjustment */
            r = 8;
            n = (n - 1) / 8;
        } else {
            n /= 8;
        }
        digits[len++] = (uint8_t)r;
    }

    /* Assemble the decimal integer from the collected digits. */
    seq_t result = 0;
    for (size_t i = 0; i < len; ++i) {
        result = result * 10 + (seq_t)digits[len - 1 - i];
    }

    return result;
}
