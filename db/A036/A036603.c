/*
 * A036603  a(n) = n! in binary.
 * https://oeis.org/A036603
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) is the decimal integer composed of the bits of n! in binary
 *            (most significant bit first)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    seq_t f = factorial(n);          /* n! */

    /* find number of bits in f (at least 1 for f = 0 or 1) */
    unsigned bitlen = 0;
    seq_t tmp = f;
    do {
        bitlen++;
        tmp >>= 1;
    } while (tmp);

    seq_t res = 0;
    while (bitlen--) {
        seq_t bit = (f >> bitlen) & 1;
        res = res * 10 + bit;
    }
    return res;
}
