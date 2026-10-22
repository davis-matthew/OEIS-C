/*
 * A004934  a(n) = floor(n*phi^19), where phi is the golden ratio, A001622.
 * https://oeis.org/A004934
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*phi^19) = floor((9349*n + sqrt(5*(4181*n)^2))/2),
 *            since phi^19 = (L(19) + F(19)*sqrt(5))/2 = (9349 +
 *            4181*sqrt(5))/2; computed as (9349*n + isqrt(5*(4181*n)^2)) / 2
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 7, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isqrt_from_above
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/isqrt_from_above.h"

seq_t a(idx_t n)
{
    seq_t m, N, s;
    if (n == 0) return (seq_t)0;
    m = (seq_t)n * 4181;
    N = m * m * 5;
    s = isqrt_from_above(N, m * 3);
    return ((seq_t)n * 9349 + s) / 2;
}
