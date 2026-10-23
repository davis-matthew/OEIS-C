/*
 * A151783  a(n) = 4^(wt(n) - 1) where wt(n) = A000120(n).
 * https://oeis.org/A151783
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4^(popcount(n) - 1) where popcount(n) is the number of
 *            1-bits in the binary representation of n
 * Range:     u8 n <= 30, u16 n <= 510, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* count bits of n */
    idx_t v = n;
    int pop = 0;
    while (v) {
        v &= v - 1;   /* delete lowest set bit */
        pop++;
    }

    /* exponent = popcount(n) - 1 >= 0 because n >= 1 */
    int exp = pop - 1;

    /* compute 4^exp */
    seq_t result = 1;
    for (int i = 0; i < exp; ++i) {
        result *= 4;
    }
    return result;
}
