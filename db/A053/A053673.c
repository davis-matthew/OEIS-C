/*
 * A053673  Least number > 1 coprime to n, n+1, n+2, n+3 and n+4.
 * https://oeis.org/A053673
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = least prime p > 1 such that gcd(p, n(n+1)(n+2)(n+3)(n+4))
 *            = 1 Computation: test successive primes starting from 7 until
 *            one is coprime to all five consecutive integers
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Simple deterministic primality test for 64‑bit numbers.
   It is adequate for the few primes we need (p ≤ about 10⁶). */
static bool is_prime_uint64(uint64_t x) {
    if (x < 2) return false;
    if (x == 2 || x == 3) return true;
    if ((x & 1) == 0) return false;
    for (uint64_t d = 3; d * d <= x; d += 2) {
        if (x % d == 0) return false;
    }
    return true;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* out of range, offset starts at 1 */

    /* For each candidate prime p, check if any of n…n+4 is divisible by p. */
    for (uint64_t p = 7; ; ++p) {
        if (!is_prime_uint64(p)) continue;

        uint64_t rem = n % p;          /* n mod p  */
        bool bad = false;
        for (idx_t i = 0; i < 5; ++i) {
            if (((rem + i) % p) == 0) {
                bad = true;
                break;
            }
        }
        if (!bad) {
            return (seq_t)p;
        }
    }
}
