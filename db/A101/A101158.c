/*
 * A101158  Let j be the smallest integer for which n+(n+1)+...+(n+j) is a
 *          square; sequence gives the squares.
 * https://oeis.org/A101158
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest square that equals n + (n+1) + ... + (n+j)
 *            for the smallest j ≥ 0 such that the sum is a perfect square
 * Range:     u8 n <= 6, u16 n <= 32, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_square
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_square.h"

seq_t a(idx_t n)
{
    seq_t tn  = (seq_t)n;   /* convert index to compute type */
    seq_t sum = 0;          /* running sum of the arithmetic sequence */

    for (seq_t j = 0; ; ++j) {
        sum += tn + j;              /* add next term n+j to the sum   */
        if (is_square(sum))         /* check if the sum is a square   */
            return sum;             /* first such sum is the required value */
    }
}
