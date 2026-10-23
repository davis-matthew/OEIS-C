/*
 * A175297  Convert n to binary. AND each respective digit of binary n and
 *          binary A030101(n), where A030101(n) is the reversal of the order
 *          of the digits in the binary representation of n (given in
 *          decimal). a(n) is the decimal value of the result.
 * https://oeis.org/A175297
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = bitwise AND of n and its binary reversal (with same bit
 *            length)
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for 1ULL literals */

seq_t a(idx_t n)
{
    /* compute the number of bits in n (n >= 1) */
    unsigned L = 0;
    idx_t tmp = n;
    while (tmp) {
        ++L;
        tmp >>= 1;
    }

    /* compute the AND of n and its reversed bit pattern */
    idx_t res = 0;
    for (unsigned i = 0; i < L; ++i) {
        idx_t hi = (n >> i) & 1ULL;                /* bit i of original number */
        idx_t lo = (n >> (L - 1 - i)) & 1ULL;      /* bit i of reversed number */
        res |= (hi & lo) << i;                    /* set the resulting bit */
    }

    return (seq_t)res;
}
