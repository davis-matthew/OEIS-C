/*
 * A236967  Expansion of g.f. (1+3*x)^2/(1-3*x)^2.
 * https://oeis.org/A236967
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(n) = 4*n*3^n for n ≥ 1
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 16, u64 n <= 35, u128 n <= 75,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"


seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t pow3 = seq_pow((seq_t)3, n);
    return (seq_t)4 * (seq_t)n * pow3;
}
