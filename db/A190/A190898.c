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
 * Helpers:   is_prime, jacobi_symbol
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

#include "../utils/jacobi_symbol.h"
#include "../utils/is_prime.h"

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
