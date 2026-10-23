/*
 * A143262  a(n) = the number of times the binary representation of the number
 *          of divisors of n occurs in the binary representation of n.
 * https://oeis.org/A143262
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = number of positions where the binary representation of
 *            the number of divisors of n occurs in the binary representation
 *            of n
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/divisor_count_u64.h"

/* Small helper to compute the bit length of an unsigned 64‑bit integer.
   For x > 0 it returns floor(log2(x)) + 1. */
static inline int bitlen_uint64(uint64_t x) {
    return x == 0 ? 1 : 64 - __builtin_clzll(x);
}

seq_t a(idx_t n) {
    /* n fits into idx_t (uint64_t).  Convert to uint64_t for bitwise ops. */
    uint64_t n64 = (uint64_t)n;

    /* Number of divisors of n (sequence term we need). */
    seq_t divs_seq = divisor_count_u64(n64);
    uint64_t d64 = (uint64_t)divs_seq;        /* safe: divisor count < 2^32 */

    /* Lengths of binary representations. */
    int len_n = bitlen_uint64(n64);
    int len_d = bitlen_uint64(d64);

    if (len_d > len_n) {
        return (seq_t)0;
    }

    /* Mask that extracts len_d low bits.  len_d is <= 64 for 64‑bit n. */
    uint64_t mask;
    if (len_d == 64) {
        mask = ~(uint64_t)0;
    } else {
        mask = ((uint64_t)1 << len_d) - 1;
    }

    seq_t count = 0;
    for (int i = 0; i <= len_n - len_d; ++i) {
        int shift = len_n - (i + len_d);
        uint64_t part = (n64 >> shift) & mask;
        if (part == d64) {
            ++count;
        }
    }
    return count;
}
