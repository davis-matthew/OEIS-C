/*
 * A079072  Sum of numbers < n having in binary representation the same number
 *          of 0's as n.
 * https://oeis.org/A079072
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of all m < n whose binary representation contains
 *            exactly the same number of zeros as n
 * Range:     u8 n <= 43, u16 n <= 739, u32 n <= 9542, u64 n <= 9542,
 *            u128 n <= 9542, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include standard integer types for builtin helpers. */
#include <stdint.h>

/* Helper: number of bits needed to represent x (at least 1) */
static inline uint64_t bitlen_uint64(uint64_t x) {
    return x == 0 ? 1 : 64 - __builtin_clzll(x);
}

/* Helper: number of zeros in binary representation of x */
static inline unsigned zeros_uint64(uint64_t x) {
    if (x == 0) return 1;                         /* representation "0" */
    return bitlen_uint64(x) - __builtin_popcountll(x);
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                          /* a(0) = 0 */

    unsigned target_zero = zeros_uint64((uint64_t)n);
    unsigned __int128 acc = 0;                     /* 128‑bit accumulation */

    for (idx_t m = 0; m < n; ++m) {
        if (zeros_uint64((uint64_t)m) == target_zero) {
            acc += (unsigned __int128)m;
        }
    }

    return (seq_t)acc;                              /* truncate if seq_t < 128 bits */
}
