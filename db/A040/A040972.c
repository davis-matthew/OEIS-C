/*
 * A040972  Primes p such that x^5 = 14 has a solution mod p.
 * https://oeis.org/A040972
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function returns the n-th prime p for which the congruence
   x^5 ≡ 14 (mod p) has a solution.  By elementary finite‑field
   theory the condition is:

   * p = 5   → always solvable (x ≡ 4 mod 5)
   * If 5 ∤ (p–1)   → the map x↦x^5 is a bijection on F_p^*,
                       hence every non‑zero element (including 14)
                       is a 5th power.
   * If 5 | (p–1)   → 14 is a 5th power iff 14^((p–1)/5) ≡ 1 (mod p).

   The implementation searches primes in ascending order, counting
   those that satisfy the above test, and returns the one that
   reaches index n (where a(1)=2).  It works for any scalar
   type seq_t and arbitrary precision seq_t. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"   /* bool is_prime_big(seq_t n); */
#include "../utils/mod_pow.h"        /* seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod); */

seq_t a(idx_t n)
{
    if (n == 0)  /* undefined; return 0 for safety */
        return 0;

    idx_t   count = 0;      /* how many qualifying primes have been found */
    seq_t   cand  = 2;      /* current candidate prime */

    while (1) {
        if (is_prime_big(cand)) {

            if (cand == 5 || ((cand % 5) != 1)) {
                /* p = 5   or   5 ∤ (p−1) → always solvable */
                ++count;
                if (count == n)
                    return cand;
            } else {
                /* 5 | (p−1) → test 14^((p−1)/5) ≡ 1 (mod p) */
                uint64_t p64 = (uint64_t)cand;          /* safe, primes in our range < 2^64   */
                uint64_t exp = (p64 - 1) / 5;           /* exponent fits in 64 bits           */
                if (mod_pow(14, exp, cand) == 1) {
                    ++count;
                    if (count == n)
                        return cand;
                }
            }
        }

        /* Advance to the next candidate (skip even numbers after 2). */
        if (cand == 2)
            cand = 3;
        else
            cand += 2;
    }
}
