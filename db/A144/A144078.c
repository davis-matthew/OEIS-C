/*
 * A144078  a(n) = the number of digits in the binary representation of n that
 *          differ from the corresponding digit in the binary reversal of n.
 *          (I.e., a(n) = number of 1's in n XOR A030101(n).)
 * https://oeis.org/A144078
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = popcount(n XOR rev(n)) where rev(n) is the binary
 *            reversal * of n using the bit length of n (no leading zeros)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Reverse the bits of u using only the lowest "len" bits. */
static inline uint64_t reverse_bits(uint64_t u, unsigned len)
{
    uint64_t rev = 0;
    unsigned i;
    for (i = 0; i < len; ++i) {
        rev = (rev << 1) | ((u >> i) & 1ULL);
    }
    return rev;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset starts at 1, unused case */

    uint64_t u = (uint64_t)n;
    /* Determine bit length without leading zeros. */
    unsigned len = 64 - __builtin_clzll(u);

    uint64_t rev  = reverse_bits(u, len);
    uint64_t diff = u ^ rev;

    /* Count set bits using GCC/Clang builtin. */
    seq_t cnt = (seq_t)__builtin_popcountll(diff);
    return cnt;
}
