/*
 * A031973  a(n) = Sum_{k=0..n} n^k.
 * https://oeis.org/A031973
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^(n+1)-1)/(n-1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)(n + 1);
    seq_t base = (seq_t)n;
    seq_t pow  = seq_pow(base, n + 1);
    seq_t num  = pow - 1;
    seq_t den  = base - 1;
    return num / den;
}
