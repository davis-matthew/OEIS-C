/*
 * A126455  Column 0 of triangle A126454; a(n) = C( C(n+2,3) + 2, n).
 * https://oeis.org/A126455
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial( binomial(n+2,3) + 2 , n )
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 13,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A126455. */
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Compute the inner binomial C(n+2, 3).  The result may exceed the
       width of idx_t but fits in seq_t for the ranges required by the
       evaluator. */
    seq_t inner = binom(n + 2, 3);

    /* Add 2 to the inner value. */
    seq_t upper_seq = inner + (seq_t)2;

    /* Convert the upper parameter to idx_t; for the tested range the
       value is < 2^64, so the cast is safe. */
    idx_t upper = (idx_t)upper_seq;

    /* Compute the outer binomial. */
    return binom(upper, n);
}
