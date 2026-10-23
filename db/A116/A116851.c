/*
 * A116851  Number of permutations of length n which avoid the patterns 321,
 *          31245.
 * https://oeis.org/A116851
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*2^n-(n+2)*(n^3+11*n+24)/12
 * Range:     u8 n <= 2, u16 n <= 13, u32 n <= 29, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* The sequence is defined only for n ≥ 1. For n = 0 we return 0. */
    if (n < 1) return 0;

    /* 2^n */
    seq_t pow2 = seq_pow((seq_t)2, n);

    /* term1 = 5 * 2^n */
    seq_t term1 = pow2 * (seq_t)5;

    /* Compute (n+2)*(n^3+11*n+24) / 12 */
    seq_t sn  = (seq_t)n;
    seq_t n2  = sn * sn;                 // n^2
    seq_t n3  = n2 * sn;                 // n^3
    seq_t part = n3 + sn * (seq_t)11 + (seq_t)24; // n^3 + 11*n + 24
    seq_t numerator = (sn + (seq_t)2) * part;     // (n+2)*(...)
    seq_t term2 = numerator / (seq_t)12;

    return term1 - term2;
}
