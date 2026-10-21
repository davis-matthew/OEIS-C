/*
 * A071559  Smallest k such that n + k and n^2 + k^2 are simultaneously
 *          primes.
 * https://oeis.org/A071559
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 642, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = the smallest positive integer k such that
   n + k and n^2 + k^2 are both prime. */
#include <stdbool.h>
#include "../utils/is_prime64.h"

seq_t a(idx_t n)
{
    /* Work with 64‑bit unsigned integers – sufficient for all n
       in the expected range (n ≤ 10 000 ⇒ n^2 + k^2 < 2^63). */
    uint64_t n64 = (uint64_t)n;

    for (idx_t k = 1; ; ++k) {
        uint64_t k64 = (uint64_t)k;
        uint64_t nk  = n64 + k64;
        uint64_t nk2 = n64 * n64 + k64 * k64;

        if (is_prime64(nk) && is_prime64(nk2))
            return (seq_t)k64;
    }
}
