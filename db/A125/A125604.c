/*
 * A125604  Minimum of the largest prime factors of a number and its two
 *          neighbors.
 * https://oeis.org/A125604
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = min{ lpf(n-1), lpf(n), lpf(n+1) } where lpf is the
 *            largest prime factor
 * Range:     u8 n <= 877, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   largest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/largest_prime_factor.h"   /* provides: uint64_t largest_prime_factor(uint64_t m) */

seq_t a(idx_t n) {
    if (n < 3) {
        /* Values below the offset are undefined; return 0. */
        return 0;
    }

    /* Cast index to 64‑bit unsigned integer for the helper. */
    uint64_t idx = (uint64_t)n;

    /* Compute largest prime factors of n-1, n, and n+1. */
    seq_t lpf_prev = (seq_t)largest_prime_factor(idx - 1);
    seq_t lpf_curr = (seq_t)largest_prime_factor(idx);
    seq_t lpf_next = (seq_t)largest_prime_factor(idx + 1);

    /* Minimum of the three values. */
    seq_t result = lpf_prev;
    if (lpf_curr < result) result = lpf_curr;
    if (lpf_next < result) result = lpf_next;

    return result;
}
