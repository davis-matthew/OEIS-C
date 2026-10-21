/*
 * A095527  a(n) = 67 written in base 13 - n.
 * https://oeis.org/A095527
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = decimal value of 67 written in base (13‑n)
 * Range:     u8 n <= 8, u16 n <= 10, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* base for representation: 13-n (must be ≥ 2 for normal bases) */
    unsigned int base = 13u - (unsigned int)n;

    /* Base 1 (unary) – write 67 as 67 ones */
    if (base == 1u) {
        seq_t r = 0;
        for (unsigned int i = 0; i < 67u; ++i)    /* 67 ones */
            r = r * 10 + 1;
        return r;
    }

    /* Invalid base (n > 12) – return 0 */
    if (base < 2u)
        return 0;

    /* Determine the digits of 67 in the given base */
    unsigned int digits[32];
    unsigned int len  = 0;
    unsigned int tmp   = 67u;
    while (tmp > 0) {
        digits[len++] = tmp % base;
        tmp /= base;
    }
    if (len == 0) {           /* should never happen for 67>0 */
        digits[0] = 0;
        len = 1;
    }

    /* Build the decimal number represented by those digits */
    seq_t result = 0;
    for (int i = (int)len - 1; i >= 0; --i)
        result = result * 10 + (seq_t)digits[i];

    return result;
}
