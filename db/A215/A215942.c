/*
 * A215942  a(n) = sigma(6*n) - 12*n.
 * https://oeis.org/A215942
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(6*n) - 12*n
 * Range:     u8 n <= 39, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sum of divisors of 6*n, then subtracts 12*n.   */
#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    /* 6*n fits in idx_t for the intended range of n. */
    idx_t m = n * (idx_t)6;

    /* Sum of divisors of m. */
    seq_t sigma = sigma_of(m);

    /* Subtract 12*n.  The value is guaranteed to be non‑negative. */
    seq_t sub = (seq_t)n * (seq_t)12;

    return sigma - sub;
}
