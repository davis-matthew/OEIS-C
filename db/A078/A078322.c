/*
 * A078322  a(n) = rad(n*rad(n)+1), where rad = A007947 (squarefree kernel).
 * https://oeis.org/A078322
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = rad(n*rad(n)+1)
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   rad
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/rad.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset 1; guards against n<1 */
    seq_t r = rad(n);               /* squarefree kernel of n */
    seq_t prod = ((seq_t)n) * r + 1; /* n * rad(n) + 1   */
    return rad((idx_t)prod);        /* rad of the result */
}
