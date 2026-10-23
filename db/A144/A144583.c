/*
 * A144583  Value of concatenation of m and e in binary representation, where
 *          n=m*2^e and m odd.
 * https://oeis.org/A144583
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = binary_concat(odd_part(n), v2(n))
 * Range:     u8 n <= 128, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where odd_part(n) = n >> v2(n), v2(n) is the exponent of the highest
   power of 2 dividing n. */

#include <stdint.h>
#include <stddef.h>

/* number of bits required to represent a non‑negative idx_t value
   in binary; returns 1 for 0. */
static inline size_t bitlen_idx(idx_t v)
{
    if (v == 0) return 1;
    size_t l = 0;
    while (v) {
        ++l;
        v >>= 1;
    }
    return l;
}

/* OEIS sequence A144583 */
seq_t a(idx_t n)
{
    /* n >= 1 by definition (offset 1) */
    idx_t tmp = n;
    idx_t e   = 0;          /* exponent of the largest power of two |
                               that divides n  (v2(n)) */
    while ((tmp & 1ULL) == 0) {
        tmp >>= 1;
        ++e;
    }
    seq_t m = (seq_t)tmp;   /* odd part of n */

    size_t bits_e = bitlen_idx(e);   /* number of bits in e (>=1) */

    /* concatenate the binary representations of m and e: */
    seq_t result = (m << bits_e) | (seq_t)e;
    return result;
}
