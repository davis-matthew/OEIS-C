/*
 * A213542  a(n) = k AND k^k, where k=2*n+1, AND is the bitwise AND operator.
 * https://oeis.org/A213542
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, look, less
 * Formula:   a(n) = (2n+1) AND (2n+1)^(2n+1) * where ^ denotes exponentiation
 *            and AND is the bitwise AND operator
 * Range:     u8 n <= 127, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes a(n) for the sequence A213542.
 * n is an idx_t (uint64_t).  The result is returned as seq_t. */
seq_t a(idx_t n)
{
    /* Compute k = 2n+1 as a 64-bit unsigned integer. */
    uint64_t k64 = (n << 1) + 1ULL;
    seq_t k = (seq_t)k64;

    /* Bit length of k (k > 0 because k is odd). */
    unsigned L = 64 - __builtin_clzll(k64);

    /* Mask with L lower bits set to 1: ((1 << L)-1).  Constructed by shifting
     * to avoid any UB for small seq_t types. */
    seq_t mask = 0;
    for (unsigned i = 0; i < L; ++i)
        mask = (mask << 1) | 1;

    /* Compute k^k modulo 2^L using modular exponentiation by squaring.  All
     * operations are performed in seq_t, so the implementation works for
     * builtin unsigned types and for arbitrary‑precision integer classes
     * that support the standard arithmetic and bitwise operators. */
    seq_t result = 1;
    seq_t base   = k;
    uint64_t exp = k64;
    while (exp)
    {
        if (exp & 1ULL)
            result = (result * base) & mask;
        base = (base * base) & mask;
        exp >>= 1ULL;
    }

    /* Finally, a(n) = k AND (k^k). */
    return result & k;
}
