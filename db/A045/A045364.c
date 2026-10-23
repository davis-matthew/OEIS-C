/*
 * A045364  Primes congruent to {1, 2, 5, 6} mod 7.
 * https://oeis.org/A045364
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime congruent to 1, 2, 5 or 6 (mod 7)
 * Range:     u8 n <= 35, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th prime that satisfies r = p mod 7 ∈ {1,2,5,6}.
   The computation is performed in a fully deterministic way: it
   iterates over successive integers, tests for primality by trial
   division up to the square root, and counts only those primes that
   have the required residue.  The function returns the result in
   type seq_t, relying on implicit conversion from idx_t (== uint64_t). */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/* Helper: trial‑division primality test for an unsigned 64‑bit integer. */
static inline bool is_prime_local(uint64_t n) {
    if (n < 2) return false;
    if ((n & 1ULL) == 0ULL) return n == 2ULL;
    for (uint64_t d = 3ULL; d <= n / d; d += 2ULL) {
        if ((n % d) == 0ULL) return false;
    }
    return true;
}

/* External linkage required by the build system. */
seq_t a(idx_t n) {
    if (n == 0ULL) return (seq_t)0;   /* safeguard – offset is 1 */

    idx_t count = 0ULL;
    uint64_t candidate = 2ULL;

    while (1) {
        if (is_prime_local(candidate)) {
            uint64_t r = candidate % 7ULL;
            if (r == 1ULL || r == 2ULL || r == 5ULL || r == 6ULL) {
                ++count;
                if (count == n) return (seq_t)candidate;
            }
        }
        if (candidate == 2ULL) candidate = 3ULL;
        else candidate += 2ULL;
    }
}
