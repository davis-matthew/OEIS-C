/*
 * A038556  Periodic derivative of n.
 * https://oeis.org/A038556
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy, look
 * Formula:   a(n) = n XOR (n rotate left 1)
 * Range:     u8 n <= 255, u16 n <= 4096, u32 n <= 4096, u64 n <= 4096,
 *            u128 n <= 4096, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Convert the index to the sequence type. */
    seq_t x = (seq_t)n;

    /* Find the number of bits in x. */
    unsigned L = 0;
    seq_t tmp = x;
    while (tmp != 0) {
        tmp >>= 1;
        ++L;
    }

    /* a(0) = 0 */
    if (L == 0) {
        return 0;
    }

    /* Rotate x left by one bit within the low L bits. */
    seq_t rot = (x << 1) | (x >> (L - 1));

    /* Mask to keep only the low L bits. */
    unsigned bits = (unsigned)(sizeof(seq_t) * 8);
    seq_t mask;
    if (L == bits) {
        mask = ~(seq_t)0;        /* all bits set */
    } else {
        mask = ((seq_t)1 << L) - 1;
    }
    rot &= mask;

    /* Return the XOR of the original and rotated value. */
    return x ^ rot;
}
