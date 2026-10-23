/*
 * A186384  Adjusted joint rank sequence of (f(i)) and (g(j)) with f(i) before
 *          g(j) when f(i)=g(j), where f(i)=5i and g(j)=j(j+1)/2 (triangular
 *          number). Complement of A186383.
 * https://oeis.org/A186384
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor((n(n+1)/2)/5) (adjusted joint rank of
 *            triangular numbers in the union of 5*i and triangular numbers)
 * Range:     u8 n <= 22, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
seq_t a(idx_t n)
{
    seq_t t = (seq_t)n * ((seq_t)n + 1) / 2; /* triangular number T_n */
    seq_t fcnt = t / 5;                      /* number of multiples of 5 <= T_n */
    return (seq_t)n + fcnt;
}
