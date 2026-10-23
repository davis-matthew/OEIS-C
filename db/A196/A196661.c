/*
 * A196661  Expansion of g.f. (1-2*x)/(1-7*x).
 * https://oeis.org/A196661
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(n) = 5*7^(n-1) for n>0 (equivalently a(n) = 7*a(n-1)
 *            for n>0)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }
    return (seq_t)5 * seq_pow((seq_t)7, n - 1);
}
