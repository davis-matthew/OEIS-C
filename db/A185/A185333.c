/*
 * A185333  Number of binary necklaces of n beads for which a cut exists
 *          producing a palindrome.
 * https://oeis.org/A185333
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(2m+1) = 2^{(n+1)/2} a(2m) = 2^{m-1} + 2^{(h-1)/2}, where h is
 *            the largest odd divisor of m
 * Range:     u8 n <= 14, u16 n <= 30, u32 n <= 62, u64 n <= 126,
 *            u128 n <= 254, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Ensure n >= 1 as per sequence definition */
    if ((n & 1) == 1)  /* odd n */
    {
        /* (n+1)/2 fits in idx_t */
        idx_t exp = (n + 1) >> 1;
        return pow2(exp);
    }
    else                /* even n */
    {
        idx_t m = n >> 1;          /* m = n/2 */
        idx_t tmp = m;
        /* find largest odd divisor of m */
        while ((tmp & 1ULL) == 0)
            tmp >>= 1;
        idx_t h = tmp;              /* h is odd */

        seq_t part1 = pow2(m - 1);                           /* 2^{m-1} */
        seq_t part2 = pow2((h - 1) >> 1);                    /* 2^{(h-1)/2} */
        return part1 + part2;
    }
}
