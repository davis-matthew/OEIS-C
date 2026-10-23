/*
 * A116702  Number of permutations of length n which avoid the patterns 123,
 *          3241.
 * https://oeis.org/A116702
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*2^(n-1) + n - (n+1)*(n+2)/2
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Handle offset 1; values for n>=1 are requested. */
    if (n == 0) return 0;

    seq_t part1 = (seq_t)3 * pow2((idx_t)(n - 1));
    seq_t N     = (seq_t)n;
    seq_t part3 = ((N + (seq_t)1) * (N + (seq_t)2)) / (seq_t)2;
    return part1 + N - part3;
}
