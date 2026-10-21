/*
 * A073759  Largest number < n that is neither a divisor of nor relatively
 *          prime to n, or 0 if no such number exists.
 * https://oeis.org/A073759
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 257, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n): largest integer m < n that is neither a divisor of n nor coprime to n;
   returns 0 if no such m exists. Brute‑force search from n-1 downwards. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd64.h"

/* Euclidean GCD on 64‑bit values */

/* Trial‑division primality test for 64‑bit values */
static inline bool is_prime_u64(uint64_t n) {
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (uint64_t d = 3; d <= n / d; d += 2) {
        if (n % d == 0) return false;
    }
    return true;
}

seq_t a(idx_t n) {
    /* No unrelated numbers for very small n */
    if (n <= 2) return 0;

    for (idx_t m = n - 1; m > 1; --m) {
        /* skip divisors of n */
        if (n % m == 0) continue;
        /* skip coprime numbers */
        if (gcd64(n, m) == 1) continue;
        /* require m to be composite */
        if (is_prime_u64((uint64_t)m)) continue;
        return (seq_t)m;
    }
    return 0;
}
