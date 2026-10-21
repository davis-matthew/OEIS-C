/*
 * A240086  a(n) = Sum_{prime p|n} phi(gcd(p, n/p)) where phi is Euler's
 *          totient function.
 * https://oeis.org/A240086
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{prime p|n} φ(gcd(p, n/p))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation factors each n by trial division, suitable
   for the required ranges (up to at least 10 000 in a few microseconds).
   It uses only standard C integer types and the seq_t/idx_t types
   supplied by the build.  No external helpers are required. */

#include <stdint.h>
#include <stddef.h>

/* Computes a(n) for n ≥ 1. */
seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                       /* a(1) = 0, values for n < 1 are irrelevant */

    uint64_t n0 = (uint64_t)n;           /* keep the original n */
    uint64_t m  = n0;                    /* mutable copy for factoring */
    uint64_t acc = 0;                    /* accumulator in 64‑bit to avoid overflow */

    /* factor 2 separately */
    if (m % 2 == 0) {
        /* skip all powers of 2 */
        while (m % 2 == 0)
            m /= 2;
        /* p = 2 is a distinct prime divisor */
        if ((n0 / 2) % 2 == 0)          /* if 4 | n0 */
            acc += (uint64_t)1;         /* phi(2) = 1 */
        else
            acc += (uint64_t)1;         /* phi(1) = 1, but for p=2 and p^2∤n we add 1 as well */
    }

    /* odd factors */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            /* skip all powers of p */
            while (m % p == 0)
                m /= p;
            /* p is a distinct prime divisor */
            if ((n0 / p) % p == 0)      /* p^2 divides n0 */
                acc += (p - 1);          /* phi(p) = p-1 */
            else
                acc += 1;                /* phi(1) = 1 */
        }
    }

    /* if something remains, it is a prime > sqrt(original n) */
    if (m > 1) {
        uint64_t p = m;
        if ((n0 / p) % p == 0)
            acc += (p - 1);
        else
            acc += 1;
    }

    return (seq_t)acc;
}
