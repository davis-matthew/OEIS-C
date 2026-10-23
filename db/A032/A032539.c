/*
 * A032539  Integer part of decimal 'base-4 looking' numbers divided by their
 *          actual base-4 values.
 * https://oeis.org/A032539
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = floor(A007090(n)/n)
 * Range:     u8 n <= 47, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    idx_t tmp = n;
    unsigned int digs[64];
    size_t len = 0;

    /* extract base‑4 digits (least significant first) */
    while (tmp > 0) {
        digs[len++] = (unsigned int)(tmp & 3);   /* tmp % 4 */
        tmp >>= 2;                               /* tmp /= 4 */
    }

    /* build the decimal number obtained from those digits */
    seq_t s = 0;
    for (size_t i = 0; i < len; ++i) {
        s = s * 10 + (seq_t)digs[len - 1 - i];
    }

    /* integer part of the division by n */
    return s / (seq_t)n;
}
