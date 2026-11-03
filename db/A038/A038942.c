/*
 * A038942  Primes p such that 61 is not a square mod p.
 * https://oeis.org/A038942
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 62 terms through n = 905 (2026-11-01)
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A038942: primes p such that 61 is not a quadratic residue modulo p
 * (including the special case p = 2). */
#include <stdbool.h>
#include <stdint.h>
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
    if (n == 0) return 0;                     /* offset is 1, ignore 0 */
    if (n == 1) return (seq_t)2;              /* the first term is 2 */

    idx_t count = 1;                          /* 2 is already counted */
    uint64_t start = 3;                        /* search from 3 upwards */

    while (1) {
        uint64_t p = next_prime(start);        /* next prime ≥ start */
        if (jacobi_symbol(61ULL, p) == -1) {   /* 61 is a non‑residue mod p */
            count++;
            if (count == n) return (seq_t)p;
        }
        start = p + 1;                         /* continue after this prime */
    }
}
