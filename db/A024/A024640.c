/*
 * A024640  n written in fractional base 7/3.
 * https://oeis.org/A024640
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is n written in base 7/3 and interpreted as a decimal
 *            integer
 * Range:     u8 n <= 20, u16 n <= 279, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Special case: n == 0 => representation is "0" */
    if (n == 0)
        return 0;

    /* Base parameters for 7/3 */
    const idx_t A = 7;
    const idx_t B = 3;

    /* The digits of n in base 7/3 (least significant first) */
    uint8_t digits[64];
    size_t len = 0;

    idx_t t = n;
    while (t > 0)
    {
        uint8_t d = t % A;          /* Current digit */
        digits[len++] = d;
        idx_t rem = t - d;          /* t - d is divisible by A */

        /* New t = (t - d) * B / A, using 128‑bit intermediate to avoid overflow */
        unsigned __int128 tmp = (unsigned __int128)rem / A;
        tmp *= B;
        t = (idx_t)tmp;
    }

    /* Interpret the digit string as a decimal integer */
    seq_t value = 0;
    for (size_t i = 0; i < len; ++i)
    {
        /* process from most significant to least */
        const uint8_t d = digits[len - 1 - i];
        value = value * 10 + d;
    }

    return value;
}
