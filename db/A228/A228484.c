/*
 * A228484  a(n) = 2^n*(3*n)!/(n!*(2*n)!).
 * https://oeis.org/A228484
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n*(3*n)!/(n!*(2*n)!)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   binomial, pow2, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/binomial.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    seq_t p2 = pow2(n);                      /* 2^n */
    seq_t b  = binomial(3 * n, n);           /* (3n choose n) */
    return seq_mul(p2, b);                   /* a(n) = 2^n * binomial(3n, n) */
}
