/*
 * A191038  Primes that are squares mod 59.
 * https://oeis.org/A191038
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   seq_t a(idx_t n) returns the n-th prime p for which (p|59) = 1
 *            (i.e. p is a quadratic residue modulo 59)
 * Range:     u8 n <= 24, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 70 terms through n = 999 (2026-11-01)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

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
    if (n == 0) return 0;          /* undefined for offset 0 */

    idx_t count = 0;               /* number of qualifying primes found */
    uint64_t p = 2;                /* current candidate prime */

    for (;; ) {
        if (is_prime(p)) {
            if (jacobi_symbol(p, 59) == 1) {
                ++count;
                if (count == n)
                    return (seq_t)p;
            }
        }
        /* advance to next odd number (2 is handled once) */
        if (p == 2)
            p = 3;
        else
            p += 2;
    }
}
