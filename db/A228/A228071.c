/*
 * A228071  Write n in binary and interpret as a decimal number; a(n) is this
 *          quantity minus n.
 * https://oeis.org/A228071
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = A007088(n) - n where A007088(n) is the decimal value
 *            obtained by reading the binary representation of n as a decimal
 *            number
 * Range:     u8 n <= 7, u16 n <= 31, u32 n <= 1023, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0U;

    /* number of bits in n (excluding leading zeros) */
    int len = 64 - __builtin_clzll((uint64_t)n);

    seq_t dec = 0;
    for (int i = len - 1; i >= 0; --i) {
        uint64_t bit = (n >> i) & 1ULL;
        dec = dec * 10 + (seq_t)bit;
    }
    return dec - (seq_t)n;
}
