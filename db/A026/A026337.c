/*
 * A026337  a(n) = 4^n*(4^n - 1)/2.
 * https://oeis.org/A026337
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4^n * (4^n - 1)) / 2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    seq_t t = pow4(n);          /* t = 4^n  */
    seq_t half = t >> 1;        /* t / 2   */
    seq_t res = half * (t - 1); /* (4^n/2)*(4^n-1) = a(n) */
    return res;
}
