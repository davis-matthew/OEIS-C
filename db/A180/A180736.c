/*
 * A180736  a(n) = [r]*[2*r]*...*[n*r], where r=sqrt(2) and []=floor.
 * https://oeis.org/A180736
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = [r]*[2*r]*...*[n*r], where r=sqrt(2) and []=floor
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 31,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;  /* Empty product */

    seq_t prod = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t k = (seq_t)i;
        seq_t term = floor_sqrt((seq_t)2 * k * k); /* floor(k*sqrt(2)) */
        prod *= term;
    }
    return prod;
}
