/*
 * A028496  Erroneous version of A152455.
 * https://oeis.org/A028496
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = φ(n) for all n ≥ 2, except a(11) = 12 (erroneous value)
 * Range:     i8 n <= 13, i16 n <= 13, i32 n <= 13, i64 n <= 13, i128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* φ(n) (Euler's totient) is the count of integers 1 ≤ k ≤ n that are coprime to n. */
/* This implementation uses a simple trial‐division factorisation. */
#include <stdint.h>
#include <stddef.h>

/* External linkage as required */
seq_t a(idx_t n)
{
    /* Special erroneous case */
    if (n == 11)
        return (seq_t)12;

    /* Compute φ(n) */
    idx_t m = n;                /* working copy of n for factorisation */
    seq_t phi = (seq_t)n;       /* start from n */

    /* Trial division up to sqrt(m) */
    for (idx_t p = 2; p * p <= m; ++p)
    {
        if (m % p == 0)
        {
            /* p is a prime factor */
            while (m % p == 0)
                m /= p;                 /* remove all powers of p */

            /* Update phi: phi = phi * (1 - 1/p) */
            phi -= phi / (seq_t)p;
        }
    }

    /* If a prime factor > sqrt(n) remains */
    if (m > 1)
        phi -= phi / (seq_t)m;

    return phi;
}
