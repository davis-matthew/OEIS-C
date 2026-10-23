/*
 * A036044  BCR(n): write in binary, complement, reverse.
 * https://oeis.org/A036044
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base, nice, look
 * Formula:   a(n) = BCR(n) – number obtained by writing n in binary,
 *            complementing each bit and reversing the resulting bit string
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   reverse_k_bits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/reverse_k_bits.h"

static inline int bitlen_uint64(uint64_t x)
{
#ifdef __GNUC__
    return x ? 63 - __builtin_clzll(x) + 1 : 0;
#else
    int len = 0;
    while (x) { ++len; x >>= 1; }
    return len;
#endif
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;          /* a(0) = 1 by definition */
    int k = bitlen_uint64(n);              /* number of bits in n */

    seq_t mask;
    if (k == 64) {
        mask = (seq_t)(~(seq_t)0);         /* all 64 bits set */
    } else {
        mask = ((seq_t)1 << k) - 1;
    }

    seq_t comp = (~((seq_t)n)) & mask;      /* complement within k bits */
    return reverse_k_bits(comp, k);         /* reverse k-bit string */
}
