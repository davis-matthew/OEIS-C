/*
 * A051072  Primes p such that x^6 = -2 has a solution mod p.
 * https://oeis.org/A051072
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   primes p such that x^6 = -2 has a solution mod p
 * Range:     u8 n <= 17, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd64, has_solution, is_prime64, mulmod64, next_prime, powmod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence a(n) lists the primes p in ascending order for which
   the congruence x^6 ≡ -2 (mod p) has a solution.
   Returns the n-th such prime (n ≥ 1). */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd64.h"
#include "../utils/has_solution.h"
#include "../utils/is_prime64.h"
#include "../utils/mulmod64.h"
#include "../utils/next_prime.h"
#include "../utils/powmod64.h"

/* ---------- helper functions --------------------------------- */

/* ---------- required sequence function ----------------------- */

/* External linkage required by the test harness */
seq_t a(idx_t n)
{
    uint64_t count = 0;
    uint64_t p = 2;

    while (true) {
        if (has_solution(p)) {
            ++count;
            if (count == n) {
                return (seq_t)p;
            }
        }
        p = next_prime(p);
    }
}
