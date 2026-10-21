/*
 * A231550  Permutation of nonnegative integers: for each bit[i] in the binary
 *          representation, except the most and the least significant bits,
 *          set bit[i] = bit[i] XOR bit[i-1], where bit[i-1] is the less
 *          significant bit, XOR is the binary logical exclusive or operator.
 * https://oeis.org/A231550
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, base, easy
 * Formula:   a(n) = permutation that for each bit i except the most *
 *            significant and least significant bits, set new_bit[i] =
 *            old_bit[i] * XOR old_bit[i-1] (e.g. Gray‑code like
 *            transformation)
 * Range:     u8 n <= 255, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* trivial cases */
    if (n <= 1)
        return (seq_t)n;

    /* determine the number of bits in n (at least 2) */
    idx_t tmp = n;
    unsigned bitlen = 0;
    while (tmp) {
        tmp >>= 1;
        bitlen++;
    }

    seq_t original = (seq_t)n;
    seq_t result   = original;

    /* process all inner bits: 1 <= i <= bitlen-2 */
    for (unsigned i = 1; i + 1 < bitlen; ++i) {
        seq_t bit_i    = (original >> i) & 1;
        seq_t bit_prev = (original >> (i - 1)) & 1;
        seq_t new_bit  = bit_i ^ bit_prev;
        if (bit_i != new_bit)
            result ^= ((seq_t)1 << i);   /* toggle the bit */
    }

    return result;
}
