/*
 * A083054  a(n) = floor(sqrt(3)*n) - 3*floor(n/sqrt(3)).
 * https://oeis.org/A083054
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*sqrt(3)) mod 3
 * Range:     u8 n <= 9, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   floor_sqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt_seq.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1. For n = 0 it is not used,
       but we return 0 just in case. */
    if (n == 0) return 0;

    /* Convert the index to the sequence type as required. */
    seq_t N = (seq_t)n;

    /* Compute floor(sqrt(3*n^2)).  Compute the product in the
       sequence type to avoid overflow in 64‑bit builds. */
    seq_t prod = N * N;
    prod = prod * ((seq_t)3);

    /* Integer square root of a seq_t value. */
    seq_t sq = floor_sqrt_seq(prod);

    /* Return the result modulo 3. */
    return sq % ((seq_t)3);
}
