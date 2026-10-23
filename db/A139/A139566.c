/*
 * A139566  a(n) is the sum of squares of digits of a(n-1); a(1)=15.
 * https://oeis.org/A139566
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_square_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=15; a(n)=sum of squares of digits of a(n-1) */
#include <stdint.h>
#include "../utils/digit_square_sum.h"

seq_t a(idx_t n)
{
    /* Base case */
    if (n == (idx_t)1)
        return (seq_t)15;

    /* Iterate from a(2) up to a(n) */
    seq_t prev = (seq_t)15;
    for (idx_t i = (idx_t)2; i <= n; ++i)
    {
        /* digit_square_sum operates on uint64_t, the values here are < 200 */
        uint64_t sum = digit_square_sum((uint64_t)prev);
        prev = (seq_t)sum;
    }
    return prev;
}
