/*
 * A059265  Primes p such that x^14 = 2 has no solution mod p.
 * https://oeis.org/A059265
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime64, powmod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A059265: primes p for which the congruence  x^14 ≡ 2 (mod p) has no solution.
    Entry for a prime p: 2 is not a 14‑th power residue modulo p
    ⇔ 2^((p-1)/gcd(p-1,14)) mod p ≠ 1.
*/

#include <stdint.h>
#include <stdbool.h>

#include "../utils/powmod64.h"   /* powmod64(uint64_t base, uint64_t exp, uint64_t mod) */
#include "../utils/is_prime64.h" /* is_prime64(uint64_t n) */

static uint64_t gcd_uint64(uint64_t a, uint64_t b)
{
    while (b != 0U) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

static bool good_prime(uint64_t p)
{
    /* p is an odd prime, p > 2 */
    uint64_t d  = gcd_uint64(p - 1U, 14ULL);
    uint64_t exp = (p - 1U) / d;
    return powmod64(2ULL, exp, p) != 1ULL;
}

seq_t a(idx_t n)
{
    /* a(1) = 3, a(2) = 5, ... */
    if (n == 0) return 0;   /* undefined, but harmless */

    idx_t  count = 0;
    uint64_t p = 3;         /* first odd prime */

    for (;;) {
        if (is_prime64(p) && good_prime(p)) {
            ++count;
            if (count == n) return (seq_t)p;
        }
        /* next odd number (p + 2) may also be composite, but is_prime64 checks it */
        p += 2;
    }
}
