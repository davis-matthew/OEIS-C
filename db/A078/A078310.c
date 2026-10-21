/*
 * A078310  a(n) = n*rad(n) + 1, where rad = A007947 (squarefree kernel).
 * https://oeis.org/A078310
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * rad(n) + 1
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   rad
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/rad.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1; return 0 for n = 0 for safety. */
    if (n == 0) return 0;

    seq_t rad_n = rad(n);              /* squarefree kernel of n */
    return (seq_t)n * rad_n + (seq_t)1; /* a(n) = n * rad(n) + 1 */
}
