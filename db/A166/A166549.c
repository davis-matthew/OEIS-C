/*
 * A166549  The number of halving steps of the Collatz 3x+1 map to reach 1
 *          starting from 2n-1.
 * https://oeis.org/A166549
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of halving steps of the Collatz 3x+1 map to reach
 *            1 starting from 2n-1
 * Range:     u8 n <= 13, u16 n <= 3327, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Handle non-positive indices safely.  The sequence is only defined for n >= 1. */
    if (n == 0)
        return 0;

    /* Compute 2n-1 as a seq_t value.  We multiply first to avoid overflow in idx_t. */
    seq_t m = (seq_t)n * 2ULL - 1;
    seq_t count = 0;

    while (m > 1) {
        /* Count all trailing divisions by 2 (halving steps). */
        while ((m & ((seq_t)1)) == 0) {
            m >>= 1;
            count++;
        }
        /* If m is still > 1, perform an odd Collatz step: 3*m + 1. */
        if (m > 1)
            m = m * 3 + 1;
    }
    return count;
}
