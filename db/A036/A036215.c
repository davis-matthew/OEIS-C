/*
 * A036215  Binary reversal of 3^n.
 * https://oeis.org/A036215
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = reverse_bits(3^n) (reverse the binary representation of
 *            3^n)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   bitlen_seq_t, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"           /* seq_t pow3(idx_t exp); */
#include "../utils/bitlen_seq_t.h"   /* unsigned bitlen_seq_t(seq_t x); */

seq_t a(idx_t n)
{
    /* Compute 3^n in the appropriate integer type. */
    seq_t power = pow3(n);

    /* Number of bits needed to represent 3^n. */
    unsigned len = bitlen_seq_t(power);

    /* Reverse the bits of power. */
    seq_t rev = 0;
    for (unsigned i = 0; i < len; ++i) {
        rev = (rev << 1) | ((power >> i) & (seq_t)1);
    }

    return rev;
}
