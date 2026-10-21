/*
 * A053642  Rotate n one binary digit to the left, drop leading zeros, then
 *          rotate one binary digit to the right.
 * https://oeis.org/A053642
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = Rotate(n left by 1 bit, drop leading zeros, then rotate
 *            right by 1 bit)
 * Range:     u8 n <= 383, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bitlen_idx, bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/bitlen_idx.h"
#include "../utils/bitlen_seq_t.h"

seq_t a(idx_t n) {
    /* find bit length of n */
    unsigned l = bitlen_idx(n);
    /* left rotation of n with bit length l */
    seq_t left = ((seq_t)n << 1) | (seq_t)(n >> (l - 1));
    /* mask to keep only l bits */
    unsigned bits_seq_t = sizeof(seq_t) * 8;
    seq_t mask;
    if (l == bits_seq_t) {
        mask = ~((seq_t)0);
    } else {
        mask = ((seq_t)1 << l) - 1;
    }
    seq_t m = left & mask;
    /* right rotation of m using its actual bit length */
    unsigned lm = bitlen_seq_t(m);
    seq_t result = (m >> 1) | ((m & 1) << (lm - 1));
    return result;
}
