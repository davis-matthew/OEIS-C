/*
 * A189469  a(n) = n + [n*s/r] + [n*t/r]; r=1, s=1+sqrt(2), t=1+sqrt(3).
 * https://oeis.org/A189469
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + ⌊n*(1+√2)⌋ + ⌊n*(1+√3)⌋ Equivalent: a(n) = 3*n +
 *            ⌊√(2*n²)⌋ + ⌊√(3*n²)⌋
 * Range:     u8 n <= 4, u16 n <= 73, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;          /* integer n as seq_t */
    seq_t n2 = N * N;            /* n² */
    seq_t f2 = floor_sqrt((seq_t)2 * n2);  /* ⌊√(2*n²)⌋ = ⌊n*√2⌋ */
    seq_t f3 = floor_sqrt((seq_t)3 * n2);  /* ⌊√(3*n²)⌋ = ⌊n*√3⌋ */

    seq_t sum = (seq_t)3 * N;    /* 3*n */
    sum += f2;
    sum += f3;
    return sum;
}
