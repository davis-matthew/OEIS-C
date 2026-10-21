/*
 * A085099  Least natural number k such that k^2 + n is prime.
 * https://oeis.org/A085099
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the smallest natural k such that k² + n is prime
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Walk through natural numbers starting at 1. */
    uint64_t k = 1;
    for (;; ++k) {
        /* k² + n will comfortably fit in 64 bits for all n up to ~10 000. */
        unsigned __int128 cand = (unsigned __int128)k * k + (unsigned __int128)n;
        if (is_prime((idx_t)cand))
            return (seq_t)k;
    }
}
