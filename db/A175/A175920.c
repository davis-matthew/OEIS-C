/*
 * A175920  Convert n to binary. XNOR each respective digit of binary n and
 *          binary A030101(n), where A030101(n) is the reversal of the order
 *          of the digits in the binary representation of n (given in
 *          decimal). a(n) is the decimal value of the result.
 * https://oeis.org/A175920
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = bitwise XNOR of n and its binary reverse. For n = 0, a(0)
 *            = 0
 * Range:     u8 n <= 256, u16 n <= 8706, u32 n <= 8706, u64 n <= 8706,
 *            u128 n <= 8706, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* find number of bits in n */
    idx_t tmp = n;
    int len = 0;
    while (tmp) {
        len++;
        tmp >>= 1;
    }

    seq_t result = 0;
    for (int i = 0; i < len; ++i) {
        /* bit i of n (LSB-first) */
        int bit_n = (int)((n >> i) & 1);
        /* bit i of reversed n = bit (len-1-i) of n */
        int bit_rev = (int)((n >> (len - 1 - i)) & 1);
        /* XNOR: bit is 1 if bits are equal */
        if (bit_n == bit_rev)
            result |= (seq_t)1 << i;
    }
    return result;
}
