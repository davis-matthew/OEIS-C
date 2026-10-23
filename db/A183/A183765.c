/*
 * A183765  1/32 the number of (2n+1) X (2n+1) binary arrays with equal
 *          numbers of 2 X 2 subblocks with sum mod two being 0 and 1.
 * https://oeis.org/A183765
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(4*(n-1)) * C(4*n^2, 2*n^2)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 5,
 *            big any n
 * Helpers:   binom, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS sequence A183765. */
#include "../utils/binom.h"   /* declares seq_t binom(idx_t, idx_t); */
#include "../utils/pow2.h"    /* declares seq_t pow2(idx_t); */

seq_t a(idx_t n)
{
    /* Guard against n < 1; the sequence is defined for n ≥ 1. */
    if (n == 0) return 0;

    /* Compute the parameters of the binomial coefficient. */
    idx_t sq   = n * n;                 /* n² */
    idx_t tot  = 4 * sq;                /* 4n²  */
    idx_t half = 2 * sq;                /* 2n²  */

    /* Binomial coefficient C(4n², 2n²). */
    seq_t bin = binom(tot, half);

    /* Power of two: 2^(4*(n‑1)). */
    idx_t shift = 4 * (n - 1);          /* exponent for 2ⁿ */
    seq_t mult = pow2(shift);

    /* Result: bin * mult.  Operator * works for both integer and
       arbitrary‑precision types supported by seq_t. */
    return bin * mult;
}
