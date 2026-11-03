/*
 * A038953  Primes p such that 70 is a square mod p.
 * https://oeis.org/A038953
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 62 terms through n = 821 (2026-11-01)
 * Helpers:   is_prime, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The OEIS sequence a(n) lists primes p such that 70 is a quadratic residue mod p.
   This implementation generates successive primes with next_prime() and keeps
   those for which the Legendre/Jacobi symbol (70|p) equals 1 or 0 (the latter
   occurs when p divides 70).  Prime 2 is handled explicitly. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/next_prime.h"

/* Jacobi symbol (a|n) for odd positive n <= 2^64-1.
   Returns 1, 0, or -1. */
static int jacobi_symbol(uint64_t a, uint64_t n) {
    if (n == 0) return 0;
    a %= n;
    if (a == 0) return 0;
    int res = 1;
    while (a != 0) {
        while ((a & 1) == 0) {
            a >>= 1;
            uint64_t n_mod8 = n & 7;
            if (n_mod8 == 3 || n_mod8 == 5)
                res = -res;
        }
        uint64_t tmp = a;
        a = n;
        n = tmp;
        if ((a & 3) == 3 && (n & 3) == 3)
            res = -res;
        a %= n;
    }
    return (n == 1) ? res : 0;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset handling */
    if (n == 1) return (seq_t)2;         /* first term is 2 */
    idx_t count = 1;                     /* we already have prime 2 */
    idx_t prime = 3;                     /* start searching from 3 */
    while (count < n) {
        if (is_prime(prime)) {
            int js = jacobi_symbol(70ULL, prime);
            if (js == 1 || js == 0) {    /* up to 70 is a quadratic residue */
                count++;
                if (count == n) return (seq_t)prime;
            }
        }
        prime = next_prime(prime + 1);
    }
    return (seq_t)0;                     /* should never be reached */
}
