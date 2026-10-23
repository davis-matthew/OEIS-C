/*
 * A186356  Adjusted joint rank sequence of (f(i)) and (g(j)) with f(i) after
 *          g(j) when f(i)=g(j), where f(i)=3i and g(j)=j(j+1)/2 (triangular
 *          number). Complement of A186357.
 * https://oeis.org/A186356
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor((sqrt(1 + 24*n) - 1)/2)
 * Range:     u8 n <= 2, u16 n <= 120, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Convert n to seq_t for arithmetic with the same type as seq_t. */
    seq_t N = (seq_t)n;

    /* Compute 1 + 24*n (must be done in seq_t to avoid overflow). */
    seq_t val = (seq_t)1 + (seq_t)24 * N;

    /* Floor square root of val. */
    seq_t s = floor_sqrt(val);

    /* j = floor((sqrt(1+24*n)-1)/2). */
    seq_t j = (s - (seq_t)1) / (seq_t)2;

    /* a(n) = n + j. */
    return N + j;
}
