/*
 * A059335  Primes p such that x^33 = 2 has no solution mod p.
 * https://oeis.org/A059335
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_pow, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by testing each prime p>2 and checking whether
 * 2^((p-1)/gcd(33,p-1)) mod p != 1. If so, p is in the
 * sequence. */
#include <stdint.h>
#include "../utils/next_prime.h"
#include "../utils/mod_pow.h"

static inline uint64_t ugcd(uint64_t a, uint64_t b)
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
    idx_t count = 0;
    uint64_t p = 2;          /* start below the first prime > 2 */
    while (count < n) {
        p = next_prime(p);   /* next prime greater than current p   */
        uint64_t d = ugcd(33ULL, p - 1);          /* gcd(33, p-1)   */
        uint64_t exp = (p - 1) / d;               /* exponent       */
        seq_t r = mod_pow((seq_t)2, exp, (seq_t)p);/* 2^exp mod p    */
        if (r != (seq_t)1) {
            ++count;
        }
    }
    return (seq_t)p;
}
