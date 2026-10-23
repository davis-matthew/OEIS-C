/*
 * A035778  Coordination sequence for C_41 lattice.
 * https://oeis.org/A035778
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = coefficient of x^(2n) in ((1+x)/(1-x))^41
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 9, u128 n <= 34,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * The coefficient can be written as a finite sum:
 *   a(n) = Σ_{i=0}^{min(41,2n)} binom(41,i) * binom(40 + 2n - i, 2n - i)
 *
 * Each term is computed using the helper function binom which handles
 * arbitrary-precision integers.  The sum uses only + and * (and subtraction)
 * so it works for all supported seq_t types.
 */
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    idx_t two_n = n * 2;
    idx_t max_i = (two_n < 41) ? two_n : 41;

    seq_t sum = 0;
    for (idx_t i = 0; i <= max_i; ++i) {
        idx_t k = two_n - i;
        seq_t t1 = binom(41, i);
        seq_t t2 = binom(40 + k, k);
        sum += t1 * t2;
    }
    return sum;
}
