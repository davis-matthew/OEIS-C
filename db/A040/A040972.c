/*
 * A040972  Primes p such that x^5 = 14 has a solution mod p.
 * https://oeis.org/A040972
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 64 terms through n = 924 (2026-11-03)
 * Helpers:   is_prime_u64, powmod
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
#include <stdlib.h>
#include <string.h>
#include "../utils/powmod.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

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
                if (powmod(14, exp, cand) == 1) {
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
