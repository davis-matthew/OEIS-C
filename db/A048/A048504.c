/*
 * A048504  a(n) = T(n,n), array T given by A048494.
 * https://oeis.org/A048504
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)*2^(n-1) + n + 1
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 23, u64 n <= 53, u128 n <= 115,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    seq_t term = (seq_t)n * ((seq_t)n - (seq_t)1);
    term = term * pow2((idx_t)(n - 1));
    term = term + (seq_t)n + (seq_t)1;
    return term;
}
