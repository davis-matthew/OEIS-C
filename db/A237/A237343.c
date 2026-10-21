/*
 * A237343  For k in {2,3,...,9} define a sequence as follows: a(0)=0; for
 *          n>=0, a(n+1)=a(n)+1, unless a(n) ends in k, in which case a(n+1)
 *          is obtained by replacing the last digit of a(n) with the digit(s)
 *          of k^2. This is k(6).
 * https://oeis.org/A237343
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n for n < 6; a(n) = (8 + 10^(n-5)) / 3 for n ≥ 6
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n < 6)
        return (seq_t)n;
    seq_t p = pow10(n - 5);
    seq_t sum = p + (seq_t)8;
    return sum / (seq_t)3;
}
