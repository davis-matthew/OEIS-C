/*
 * A186357  Adjusted joint rank sequence of (f(i)) and (g(j)) with f(i) after
 *          g(j) when f(i)=g(j), where f(i)=3i and g(j)=j(j+1)/2 (triangular
 *          number). Complement of A186356.
 * https://oeis.org/A186357
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor((n*(n+1)/2 - 1)/3) for n >= 1
 * Range:     u8 n <= 22, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t t   = nn * (nn + (seq_t)1) / (seq_t)2;   /* n*(n+1)/2 */
    seq_t part = (t - (seq_t)1) / (seq_t)3;        /* floor((t-1)/3) */
    return nn + part;
}
