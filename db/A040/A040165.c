/*
 * A040165  Primes p such that x^5 = 5 has a solution mod p.
 * https://oeis.org/A040165
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 44, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence of primes p such that the congruence x^5 ≡ 5 (mod p) has a solution.
   a(1)=2, a(2)=3, a(3)=5, … (OEIS A040165) */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/next_prime.h"      /* uint64_t next_prime(uint64_t start); */

static uint64_t mod_mul_u64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (unsigned __int128)a * b % mod;
}

static uint64_t powmod_u64(uint64_t base, uint64_t exp, uint64_t mod)
{
    if (mod == 1) return 0;
    uint64_t res = 1;
    base %= mod;
    while (exp) {
        if (exp & 1) res = mod_mul_u64(res, base, mod);
        base = mod_mul_u64(base, base, mod);
        exp >>= 1;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;       /* guard for offset */

    uint64_t count = 0;
    uint64_t prime = 2;

    while (count < n) {
        bool good;
        if (prime == 5) {
            good = true;
        } else if (prime % 5 != 1) {
            good = true;
        } else {
            uint64_t exp = (prime - 1) / 5;
            good = (powmod_u64(5, exp, prime) == 1);
        }

        if (good) {
            count++;
            if (count == n) break;
        }

        /* move to next prime */
        prime = (prime == 2) ? 3ULL : next_prime(prime);
    }

    return (seq_t)prime;
}
