/*
 * A216774  Primes p such that x^58 = -2 has no solution mod p.
 * https://oeis.org/A216774
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_pow, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th prime p such that the congruence x^58 = -2 (mod p) has no solution (search). */

#include <stdint.h>
#include "../utils/next_prime.h"   /* uint64_t next_prime(uint64_t start); */
#include "../utils/mod_pow.h"     /* seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod); */

/* Euclidean gcd for 64‑bit integers. */
static uint64_t gcd_u64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset is 1; unused case */

    idx_t count = 0;
    uint64_t cur = 1;      /* start before the first prime (2) */

    while (1) {
        cur = next_prime(cur);          /* next prime > cur */
        if (cur == 2) continue;         /* 2 never satisfies the condition */

        uint64_t rem = (cur - 1) % 58;
        uint64_t d   = gcd_u64(58, rem);          /* gcd(58, cur-1) */
        uint64_t exp = (cur - 1) / d;              /* exponent to test */

        seq_t residue = mod_pow((seq_t)cur - 2, exp, (seq_t)cur);
        if (residue != (seq_t)1) {
            ++count;
            if (count == n) {
                return (seq_t)cur;
            }
        }
    }
}
