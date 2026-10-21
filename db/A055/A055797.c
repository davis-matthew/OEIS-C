/*
 * A055797  T(2n+4,n), array T as in A055794.
 * https://oeis.org/A055797
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n + 5, 7) + binomial(n + 5, 5)
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 69, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements OEIS A055797 using the provided binomial helper. */
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    return binomial(n + 5, 7) + binomial(n + 5, 5);
}
