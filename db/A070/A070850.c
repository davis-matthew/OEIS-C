/*
 * A070850  Smallest prime == 1 mod (6n).
 * https://oeis.org/A070850
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 950, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the smallest prime p such that p ≡ 1 (mod 6n)
   by brute‑force search. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset is 1, but guard */
    uint64_t modulus = 6ULL * (uint64_t)n; /* 6n fits in 64 bits for practical n */
    uint64_t x = 1;
    while (1) {
        unsigned __int128 tmp = (unsigned __int128)modulus * x + 1;
        uint64_t candidate = (uint64_t)tmp;     /* value fits in 64 bits for reasonable n */
        if (is_prime_u64(candidate))
            return (seq_t)candidate;
        ++x;
    }
}
