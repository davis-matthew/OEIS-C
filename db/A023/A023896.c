/*
 * A023896  Sum of positive integers in smallest positive reduced residue
 *          system modulo n. a(1) = 1 by convention.
 * https://oeis.org/A023896
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = n * phi(n) / 2 for n > 1; a(1) = 1 where phi(n) is
 *            Euler's totient function
 * Range:     u8 n <= 28, u16 n <= 366, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 72 terms through n = 9699 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Compute Euler's totient φ(n) for n ≥ 0 (n is given as idx_t). */
static seq_t phi_from_idx(idx_t n)
{
    if (n <= 1)
        return 1;

    seq_t phi = (seq_t)n;
    idx_t temp = n;

    /* trial division up to sqrt(temp) */
    for (idx_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            /* φ(n) = φ(n)/p * (p-1) for each distinct prime factor p */
            phi = phi / (seq_t)p * (seq_t)(p - 1);

            /* remove all occurrences of p from temp */
            while (temp % p == 0)
                temp /= p;
        }
    }

    /* if temp is now a prime > 1 */
    if (temp > 1)
        phi = phi / (seq_t)temp * (seq_t)(temp - 1);

    return phi;
}

seq_t a(idx_t n)
{
    if (n == 1)
        return 1;

    seq_t phi = phi_from_idx(n);
    return (seq_t)n * phi / 2;
}
