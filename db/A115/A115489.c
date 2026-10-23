/*
 * A115489  Number of monic irreducible polynomials of degree 3 in GF(2^n)[x].
 * https://oeis.org/A115489
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/3)*(8^n - 2^n) for n >= 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This uses the provided big_pow routine for arbitrary‑precision arithmetic */
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* The OEIS offset is 1, so a(1) is the first term.  For safety, return 0 for n<1. */
    if (n < 1) return 0;

    seq_t pow2 = big_pow((seq_t)2, (seq_t)n);   /* 2^n  */
    seq_t pow8 = big_pow((seq_t)8, (seq_t)n);   /* 8^n  */

    return (pow8 - pow2) / (seq_t)3;
}
