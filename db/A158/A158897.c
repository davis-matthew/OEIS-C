/*
 * A158897  The elements of A059100 at indices of triangular numbers, padded
 *          with zeros.
 * https://oeis.org/A158897
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (i+1)^2 + 2 if n = T_i = i*(i+1)/2 for some i ≥ 1 0
 *            otherwise where i satisfies 8*n + 1 = (2*i + 1)^2
 * Range:     u8 n <= 7, u16 n <= 97, u32 n <= 97, u64 n <= 97, u128 n <= 97,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t d  = (seq_t)1 + (seq_t)8 * nn;
    seq_t s  = floor_sqrt(d);

    if (s * s != d)
        return (seq_t)0;
    if ((s & (seq_t)1) == (seq_t)0)
        return (seq_t)0;

    seq_t i = (s - (seq_t)1) / (seq_t)2;
    seq_t k = i + (seq_t)1;
    return k * k + (seq_t)2;
}
