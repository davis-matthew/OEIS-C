/*
 * A051072  Primes p such that x^6 = -2 has a solution mod p.
 * https://oeis.org/A051072
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   primes p such that x^6 = -2 has a solution mod p
 * Range:     u8 n <= 17, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 73 terms through n = 965 (2026-11-03)
 * Helpers:   gcd_u64, is_prime_u64, mulmod, next_prime, powmod_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence a(n) lists the primes p in ascending order for which
   the congruence x^6 ≡ -2 (mod p) has a solution.
   Returns the n-th such prime (n ≥ 1). */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd_u64.h"
#include "../utils/is_prime_u64.h"
#include "../utils/mulmod.h"
#include "../utils/next_prime.h"
#include "../utils/powmod_u64.h"

/* Computes whether x^6 ≡ -2 (mod p) has a solution for a prime p.
   The condition is equivalent to (p-2)^((p-1)/gcd(6,p-1)) ≡ 1 (mod p). */
static bool has_solution(uint64_t p)
{
    if (p == 2)                 return true;   /* 0^6 ≡ -2 (mod 2) */
    uint64_t d   = gcd_u64(6ULL, p - 1);
    uint64_t exp = (p - 1) / d;
    return powmod_u64(p - 2, exp, p) == 1;
}

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
