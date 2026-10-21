/*
 * A060602  Number of tilings of the d-dimensional zonotope constructed from
 *          d+3 vectors.
 * https://oeis.org/A060602
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = 2 * ((7 + n) * 2^n - n - 3)
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 25, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
/* Number of tilings of the d‑dimensional zonotope constructed from d+3 vectors */
seq_t a(idx_t n)
{
    seq_t pow2n = pow2(n);                     /* 2^n */
    seq_t term  = ((seq_t)7 + (seq_t)n) * pow2n;/* (7 + n) * 2^n */
    term -= (seq_t)n;                          /* subtract n */
    term -= (seq_t)3;                          /* subtract 3 */
    return term * (seq_t)2;                    /* multiply by 2 */
}
