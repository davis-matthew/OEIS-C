/*
 * A218290  Multiples of 5 such that the sum of their digits is also a
 *          multiple of 5.
 * https://oeis.org/A218290
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: multiples of 5 whose digit sum is also a multiple of 5.
   a(1) = 5, a(2) = 50, a(3) = 55, …  (offset 1) */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/digit_sum.h"

/*
 * Compute the nth element of the sequence.
 * The implementation performs a simple linear scan of all multiples
 * of 5, counting only those whose digit sum is also a multiple of 5.
 * This approach is O(n) in the worst case and works for all
 * supported seq_t types.
 */
seq_t a(idx_t n)
{
    if (n == 0)               /* undefined offset, return zero */
        return (seq_t)0;

    idx_t found = 0;           /* number of valid terms seen so far */

    /* Scan multiples of 5 from 5 upwards */
    for (idx_t num = 5;; num += 5)
    {
        /* Convert to seq_t for digit_sum calculation */
        seq_t x = (seq_t)num;

        /* If the digit sum of x is a multiple of 5, count it */
        if ((digit_sum(x) % 5) == 0)
        {
            ++found;
            if (found == n)
                return x;    /* nth term found */
        }
    }
}
