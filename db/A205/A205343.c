/*
 * A205343  Number of length 6 nonnegative integer arrays starting and ending
 *          with 0 with adjacent elements unequal but differing by no more
 *          than n.
 * https://oeis.org/A205343
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*(12*C(n+2,4) + 9*C(n+1,4) + 2*C(n,4))
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   choose4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/choose4.h"
/* where C(k,4) = k*(k-1)*(k-2)*(k-3)/24 if k≥4, else 0 */

seq_t a(idx_t n)
{
    seq_t t1 = choose4(n + 2);
    seq_t t2 = choose4(n + 1);
    seq_t t3 = choose4(n);
    seq_t sum = 12 * t1 + 9 * t2 + 2 * t3;
    return 2 * sum;
}
