/*
 * A222346  Number of (n+2) X 1 arrays of occupancy after each element moves
 *          up to +-n places including 0.
 * https://oeis.org/A222346
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binom(2*n+3, n+1) - 2
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    idx_t num  = 2*n + 3;   /* upper index of the binomial coefficient   */
    idx_t k    = n + 1;     /* lower index (k = n+1)                      */
    seq_t res  = binom(num, k);  /* compute C(2n+3, n+1)                      */
    res -= (seq_t)2;            /* subtract the two forbidden arrangements   */
    return res;
}
