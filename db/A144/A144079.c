/*
 * A144079  a(n) = the number of digits in the binary representation of n that
 *          equal the corresponding digit in the binary reversal of n. (I.e.,
 *          a(n) = number of 0's in n XOR A030101(n).)
 * https://oeis.org/A144079
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = number of binary digits of n that match the corresponding
 *            digits of its binary reversal (within its bit length). This
 *            equals L - popcount(n XOR rev(n)), where L is the bit length of
 *            n
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* seq_t: unsigned integer type (size depends on the build)
   idx_t: unsigned integer type (uint64_t) */
seq_t a(idx_t n)
{
    /* The OEIS offset is 1; n=0 is unused but handled defensively. */
    if (n == 0)
        return 0;

    /* 1.  Compute the bit length L of n. */
    idx_t len = 0;
    idx_t tmp = n;
    while (tmp) {
        len++;
        tmp >>= 1;
    }

    /* 2.  Compute the reverse of n over its L bits. */
    idx_t rev = 0;
    tmp = n;
    for (idx_t i = 0; i < len; ++i) {
        rev = (rev << 1) | (tmp & 1);
        tmp >>= 1;
    }

    /* 3.  XOR n with its reverse. */
    idx_t xr = n ^ rev;

    /* 4.  Count the differing bits using the standard pop‑count technique. */
    unsigned diff = 0;
    while (xr) {
        diff++;
        xr &= xr - 1;   /* clear the lowest set bit */
    }

    /* 5.  Equal bits = total bits – differing bits. */
    return (seq_t)(len - diff);
}
