/*
 * A100073  Number of representations of n as the difference of two positive
 *          squares.
 * https://oeis.org/A100073
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a_(n) where a_(n) = A056924(n) if n is odd, A056924(n/4)
 *            if 4 divides n, 0 otherwise. A056924(n) = floor(numdiv(n)/2) for
 *            odd n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/count_divisors.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    /* n >= 1 by definition of the sequence */
    if ((n & 1U) != 0)                       /* n is odd */
    {
        return count_divisors(n) / 2U;
    }
    else if ((n & 3U) != 0)                  /* 4 does not divide n */
    {
        return 0;
    }
    else                                     /* n divisible by 4 */
    {
        return count_divisors(n >> 2) / 2U;
    }
}
