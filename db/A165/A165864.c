/*
 * A165864  Totally multiplicative sequence with a(p) = 43.
 * https://oeis.org/A165864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = 43^Ω(n) where Ω(n) is the total number of prime factors
 *            of n with multiplicity
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 63, u64 n <= 4095,
 *            u128 n <= 10000, big any n
 * Helpers:   bigomega_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: factorize n using trial division, compute Ω(n), then return 43 raised to that power. */

#include <stdint.h>
#include "../utils/bigomega_idx.h"

/* Compute base^exp where base fits in uint64_t and exp is an idx_t. */
static inline seq_t pow_uint(uint64_t base, idx_t exp)
{
    seq_t result = 1;
    seq_t b = (seq_t)base;

    while (exp > 0) {
        if (exp & 1ULL) {
            result *= b;
        }
        exp >>= 1;
        if (exp > 0) {
            b *= b;
        }
    }
    return result;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n <= 1) {
        /* a(1) = 1 by definition (Ω(1) = 0) */
        return 1;
    }

    idx_t omega = bigomega_idx(n);      /* compute Ω(n) */
    return pow_uint(43ULL, omega);      /* return 43^Ω(n) */
}
