/*
 * A093216  a(n) is the largest number such that all of a(n)'s length-n
 *          substrings are distinct and divisible by 16.
 * https://oeis.org/A093216
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, less
 * Formula:   a(n) = 10^(2n) - 16*10^n for n≥4; a(1)=0, a(2)=964800,
 *            a(3)=99288000
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   mul_small, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)0;
    if (n == 2)
        return (seq_t)964800;
    if (n == 3)
        return (seq_t)99288000;

    /* n >= 4 */
    seq_t ten2n = pow10((idx_t)(2ULL * n));
    seq_t tenn  = pow10((idx_t)n);
    seq_t prod  = mul_small(tenn, 16);
    return ten2n - prod;
}
