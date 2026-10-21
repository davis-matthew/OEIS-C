/*
 * A220853  Denominators of the fraction (30*n+7) * binomial(2*n,n)^2 *
 *          2F1([1/2 - n/2, -n/2], [1], 64)/(-256)^n, where 2F1 is the
 *          hypergeometric function.
 * https://oeis.org/A220853
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = 2^(8*n - 2*popcount(n))
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 16,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    // popcount of the 64‑bit integer n
    unsigned int pc = __builtin_popcountll((unsigned long long)n);

    // exponent of 2 in the denominator
    idx_t exp = 8 * n - 2 * pc;

    // compute 2^exp
    return pow2(exp);
}
