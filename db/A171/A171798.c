/*
 * A171798  a(n) = base-10 concatenation XYZ, where X = number of bits in
 *          binary expansion of n, Y = number of 0's, Z = number of 1's.
 * https://oeis.org/A171798
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = concatenation of X, Y, Z * X = number of bits in binary
 *            expansion of n * Y = number of 0's in that binary expansion * Z
 *            = number of 1's in that binary expansion
 * Range:     u8 n <= 3, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Count bits, zeroes and ones in n */
    idx_t t = n;
    unsigned ones  = 0;
    unsigned bits  = 0;
    while (t) {
        bits++;
        ones += t & 1;
        t >>= 1;
    }
    unsigned zeros = bits - ones;

    /* Number of decimal digits of Y and Z (each ≤ 64 → at most 2 digits) */
    unsigned lenY = (zeros < 10) ? 1 : 2;
    unsigned lenZ = (ones  < 10) ? 1 : 2;

    /* Powers of ten up to 10^4 (fits in any seq_t) */
    static const seq_t pow10[5] = {1, 10, 100, 1000, 10000};

    seq_t pow10_z   = pow10[lenZ];          /* 10^lenZ                */
    seq_t pow10_yz  = pow10[lenY + lenZ];   /* 10^(lenY+lenZ)         */

    /* Concatenate X, Y, Z as decimal */
    seq_t result = (seq_t)bits  * pow10_yz
                 + (seq_t)zeros * pow10_z
                 + (seq_t)ones;
    return result;
}
