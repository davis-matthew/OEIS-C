/*
 * A198082  Ceiling(n*Sqrt(5)).
 * https://oeis.org/A198082
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(n*sqrt(5))
 * Range:     u8 n <= 3, u16 n <= 57, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed as: if n==0 return 0 else floor_sqrt(5*n^2) + 1 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* n^2 */
    seq_t nn = (seq_t)n;
    seq_t prod = nn * nn;

    /* 5 * n^2 */
    prod = prod * (seq_t)5;

    /* floor of sqrt(5*n^2) */
    seq_t flo = floor_sqrt(prod);

    /* ceil = floor + 1 for n>0 */
    return flo + (seq_t)1;
}
