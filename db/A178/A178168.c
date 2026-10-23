/*
 * A178168  Product of the numbers in the Collatz (3x+1) trajectory of n,
 *          including n.
 * https://oeis.org/A178168
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of the numbers in the Collatz (3x+1) trajectory
 *            of n, including n
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 6, u64 n <= 8, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    if (n == 0) return 0; /* According to OEIS, offset is 1; n >= 1. */
    seq_t cur = (seq_t)n;
    seq_t prod = 1;

    while (cur != 1) {
        prod *= cur;
        if ((cur & 1) == 0)
            cur /= 2;
        else
            cur = cur * 3 + 1;
    }
    /* Include the final 1; multiplication by 1 is a NOP but keeps the algorithm
       symmetric with the loop. */
    prod *= cur; /* cur == 1 */
    return prod;
}
