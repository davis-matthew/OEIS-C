/*
 * A190898  Least odd prime p>n^2 with (n/p) = 1, where ( / ) is the Legendre
 *          symbol.
 * https://oeis.org/A190898
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 9843 (2026-11-01)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Algorithm: find the least odd prime p > n^2 such that the Legendre
 * symbol (n/p) = 1.  Since the Legendre symbol is defined for odd
 * primes only, we skip the even prime 2.  Starting from the first
 * integer > n^2, we step over odd candidates, test primality, and
 * evaluate the Jacobi symbol, which equals the Legendre symbol when
 * the modulus is an odd prime.
 */
#include <stdint.h>
#include <stdbool.h>

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

/* seq_t and idx_t are defined by the build system. */
seq_t a(idx_t n)
{
    /* Compute n^2 in 64‑bit arithmetic.  All practical n are small. */
    uint64_t nsq = (uint64_t)n * (uint64_t)n;

    /* Start at the smallest integer > n^2.  Ensure we begin at an odd
     * candidate, skipping the even prime 2. */
    uint64_t p = nsq + 1;
    if (p <= 2)
        p = 3;          /* the first odd prime */
    else if ((p & 1ULL) == 0ULL)
        ++p;            /* make p odd */

    for (;;) {
        /* Skip non‑primes. */
        if (!is_prime(p)) {
            p += 2;      /* only odd candidates beyond 2 are needed */
            continue;
        }

        /* For an odd prime p, (n/p) == jacobi_symbol(n,p). */
        if (jacobi_symbol((uint64_t)n, p) == 1)
            return (seq_t)p;

        p += 2;          /* next odd number */
    }
}
