/*
 * A186344  Adjusted joint rank sequence of (f(i)) and (g(j)) with f(i) after
 *          g(j) when f(i)=g(j), where f and g are the pentagonal numbers and
 *          the octagonal numbers. Complement of A186345.
 * https://oeis.org/A186344
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor((2 + floor_sqrt(4 + 12 * (n*(3*n-1)/2)))/6)
 * Range:     u8 n <= 1, u16 n <= 30, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t i = (seq_t)n;
    seq_t P = i * ((seq_t)3 * i - (seq_t)1) / (seq_t)2;   /* Pentagonal number P(n) = n(3n-1)/2 */
    seq_t D = ((seq_t)4) + ((seq_t)12) * P;               /* D = 4 + 12*P */
    seq_t sqrtD = floor_sqrt(D);
    seq_t k = ((seq_t)2 + sqrtD) / (seq_t)6;              /* Count of octagonal numbers <= P(n) */
    return i + k;                                         /* Rank of the pentagonal number */
}
