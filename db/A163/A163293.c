/*
 * A163293  a(n) = n-th prime minus (number of bits in binary expansion of
 *          n-th prime).
 * https://oeis.org/A163293
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = A000040(n) - A035100(n) – nth prime minus number of bits
 *            in its binary expansion
 * Range:     u8 n <= 54, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   bitlen_seq_t, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/bitlen_seq_t.h"

seq_t a(idx_t n)
{
    seq_t prime = (seq_t)get_prime(n);
    seq_t bitlen = bitlen_seq_t(prime);
    return prime - bitlen;
}
