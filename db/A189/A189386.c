/*
 * A189386  a(n) = n+[ns/r]+[nt/r]; r=1, s=sqrt(2), t=1/sqrt(3), []=floor.
 * https://oeis.org/A189386
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(n*sqrt(2)) + floor(n / sqrt(3)) which is
 *            equivalent to a(n) = n + floor_sqrt(2*n*n) + floor_sqrt(3*n*n) /
 *            3
 * Range:     u8 n <= 4, u16 n <= 73, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Cast n to seq_t to avoid overflow when squaring on small types */
    seq_t ns = (seq_t)n;
    seq_t nsq = ns * ns;                     /* n^2 */
    seq_t part1 = floor_sqrt(nsq * (seq_t)2);   /* floor(sqrt(2*n^2)) = floor(n*sqrt(2)) */
    seq_t part2 = floor_sqrt(nsq * (seq_t)3) / (seq_t)3; /* floor(n/sqrt(3)) */
    return ns + part1 + part2;
}
