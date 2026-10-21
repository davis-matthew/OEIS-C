/*
 * A050297  Number of scalars which can be constructed from the Riemann tensor
 *          and metric tensor in n dimensions.
 * https://oeis.org/A050297
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)*(n-2)*(n+3)/12 for n >= 3; a(1) = 0, a(2) = 1
 * Range:     u8 n <= 7, u16 n <= 29, u32 n <= 476, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;

    /* compute using 128‑bit intermediate to avoid overflow for all builds */
    __uint128_t prod = (__uint128_t)n * (n - 1) * (n - 2) * (n + 3);
    prod /= 12;
    return (seq_t)prod;
}
