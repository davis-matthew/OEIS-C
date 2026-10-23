/*
 * A159780  Inner product of the binary representation of n and its reverse.
 * https://oeis.org/A159780
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of positions i where the i‑th bit of n (MSB first)
 *            * equals 1 and the mirrored bit (from LSB) also equals 1. * That
 *            is, a(n) = Σ_{i=0}^{len-1} (bit_MSB[i] & bit_LSB[len-1-i]), *
 *            where len = floor(log2(n)) + 1, and for n = 0 the value is 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* a(0) = 0 */

    /* compute bit length of n (number of bits in its binary representation) */
    int len = 0;
    idx_t tmp = n;
    while (tmp > 0) {
        tmp >>= 1;
        len++;
    }

    seq_t count = 0;

    /* mask for the most significant bit (MSB) */
    idx_t msb_mask = ((idx_t)1) << (len - 1);

    for (int i = 0; i < len; ++i) {
        /* bit from MSB side */
        int b1 = (n & msb_mask) ? 1 : 0;
        /* bit from LSB side */
        int b2 = ((n >> i) & 1) ? 1 : 0;
        if (b1 && b2)
            count++;

        msb_mask >>= 1; /* move to next less significant bit from MSB side */
    }

    return count;
}
