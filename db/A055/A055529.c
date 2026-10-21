/*
 * A055529  Number of bits needed in mantissa to express n! exactly.
 * https://oeis.org/A055529
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( Σ_{i=1..n} log2(i) – Σ_{k≥1} ⌊n/2^k⌋ ) + 1 This is
 *            the number of bits required in a binary floating‑point mantissa
 *            to represent n! exactly
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    /* Sum of log2(i) for i = 1..n */
    long double sum_log = 0.0L;
    for (idx_t i = 1; i <= n; ++i)
        sum_log += log2l((long double)i);

    /* Exponent of 2 in n! = Σ_{k≥1} ⌊n / 2^k⌋ */
    idx_t v2 = 0;
    for (idx_t div = 2; div <= n; div <<= 1)
        v2 += n / div;

    /* log2 of the odd part of n! */
    long double odd_log = sum_log - (long double)v2;

    /* Compute floor[odd_log] + 1 safely; add a tiny epsilon to fight
       floating‑point rounding that might bring the value slightly
       below an integer like 14.9999999999 */
    long double eps = 1e-12L;
    idx_t bits = (idx_t)(floor(odd_log + eps)) + 1;

    return (seq_t)bits;
}
