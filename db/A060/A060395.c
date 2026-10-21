/*
 * A060395  Smallest prime that divides k^2 + k + n for k = 0, 1, 2, ....
 * https://oeis.org/A060395
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 101, u16 n <= 101, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A060395: smallest prime that divides k² + k + n for some k ≥ 0.
   For n ≡ {0,2,4} (mod 6) the answer is 2, for n ≡ {1,3} (mod 6) it is 3.
   For n ≡ 5 (mod 6) we search primes p starting at 5, testing whether
   the congruence k² + k + n ≡ 0 (mod p) has a solution.  The first such
   prime is returned. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    uint64_t n64 = (uint64_t)n;
    uint64_t r = n64 % 6U;

    if (r == 0U || r == 2U || r == 4U) return (seq_t)2;
    if (r == 1U || r == 3U) return (seq_t)3;

    /* n ≡ 5 (mod 6) */
    for (uint64_t p = 5ULL; ; p += 2) {
        if (!is_prime_small(p)) continue;   /* skip non‑primes */

        uint64_t n_mod_p = n64 % p;

        /* Quick check: k = 0 */
        if (n_mod_p == 0ULL) return (seq_t)p;

        /* Test all k modulo p */
        for (uint64_t k = 0ULL; k < p; ++k) {
            if ((k * k + k + n_mod_p) % p == 0ULL) return (seq_t)p;
        }
    }
}
