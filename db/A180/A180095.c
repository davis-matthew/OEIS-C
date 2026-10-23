/*
 * A180095  a(n) = smallest number k such that three consecutive prime numbers
 *          prime(n), prime(n+1) and prime(n+2) are divisors of k, k+1 and k+2
 *          respectively.
 * https://oeis.org/A180095
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest integer k such that * prime(n) divides k, *
 *            prime(n+1) divides k+1, * prime(n+2) divides k+2 * * Computation
 *            is by Chinese Remainder Theorem on the congruences * k ≡ 0 (mod
 *            p0) * k ≡ p1-1 (mod p1) * k ≡ p2-2 (mod p2) * * The routine uses
 *            Garner's algorithm with 128‑bit arithmetic...
 * Range:     u8 n <= 3, u16 n <= 14, u32 n <= 261, u64 n <= 1227,
 *            u128 n <= 1227, big any n
 * Helpers:   get_prime, inv_mod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"    /* provides: uint32_t get_prime(idx_t k) */
#include "../utils/inv_mod.h"

seq_t a(idx_t n)
{
    /* Retrieve the three consecutive primes.  (1‑based index) */
    uint64_t p0 = (uint64_t)get_prime(n);
    uint64_t p1 = (uint64_t)get_prime(n + 1);
    uint64_t p2 = (uint64_t)get_prime(n + 2);

    /* Garner's algorithm: first combine p0 and p1 */
    uint64_t inv_p0_mod_p1 = inv_mod(p0 % p1, p1);
    uint64_t t1 = ((p1 - 1ULL) * inv_p0_mod_p1) % p1;
    /* partial solution modulo p0·p1 */
    uint64_t r1 = p0 * t1;               /* fits in 64 bits for n≤10000 */

    /* Combine with p2 */
    uint64_t m01_mod_p2 = (( (__uint128_t)p0 * p1 ) % p2);
    uint64_t inv_m01_mod_p2 = inv_mod(m01_mod_p2, p2);
    uint64_t r1_mod_p2 = r1 % p2;
    uint64_t desired_r3 = p2 - 2ULL;     /* k ≡ p2-2 (mod p2) */
    uint64_t diff = (desired_r3 >= r1_mod_p2)
                    ? (desired_r3 - r1_mod_p2)
                    : (desired_r3 + p2 - r1_mod_p2);
    uint64_t t2 = (diff * inv_m01_mod_p2) % p2;

    /* Final solution: k = r1 + (p0·p1)·t2 */
    __uint128_t part = ( (__uint128_t)p0 * p1 ) * t2;
    __uint128_t k128 = part + r1;
    return (seq_t)k128;
}
