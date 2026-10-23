/*
 * A163494  a(n) = (4*n)!*((2*n)!)^2.
 * https://oeis.org/A163494
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n)! * ((2*n)!)^2
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 5,
 *            big any n
 * Helpers:   factorial, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    seq_t f4 = factorial(4 * n);        /* (4*n)!           */
    seq_t f2 = factorial(2 * n);        /* (2*n)!           */
    seq_t f2_sq = seq_mul(f2, f2);      /* ((2*n)!)^2       */
    return seq_mul(f4, f2_sq);          /* a(n)             */
}
