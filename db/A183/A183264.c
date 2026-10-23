/*
 * A183264  Number of singly defective permutations of 1..n with exactly 1
 *          maximum.
 * https://oeis.org/A183264
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(3*n-4)*2^(n-3) for n>1, a(1)=0
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 24, u64 n <= 53, u128 n <= 115,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;
    if (n == 2)
        return (seq_t)2;
    seq_t tn = (seq_t)n;
    seq_t term = tn * (((seq_t)3 * tn) - (seq_t)4);
    term *= pow2(n - 3);
    return term;
}
