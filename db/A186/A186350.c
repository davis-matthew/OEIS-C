/*
 * A186350  Adjusted joint rank sequence of (f(i)) and (g(j)) with f(i) before
 *          g(j) when f(i)=g(j), where f and g are the odd numbers and the
 *          triangular numbers. Complement of A186351.
 * https://oeis.org/A186350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor( (sqrt(16*n - 9) - 1) / 2 )
 * Range:     u8 n <= 4, u16 n <= 120, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t temp = ((seq_t)n * ((seq_t)16)) - ((seq_t)9);
    seq_t s = floor_sqrt(temp);
    seq_t add = (s - ((seq_t)1)) / ((seq_t)2);
    return ((seq_t)n) + add;
}
