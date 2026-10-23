/*
 * A186498  Adjusted joint rank sequence of (f(i)) and (g(j)) with f(i) before
 *          g(j) when f(i)=g(j), where f(i)=3i-2 and g(j)=j-th triangular
 *          number. Complement of A186497.
 * https://oeis.org/A186498
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor((n(n+1)/2 + 2)/3)
 * Range:     u8 n <= 22, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* not used (offset=1) */
    seq_t tn = (seq_t)n;
    seq_t t = tn * (tn + 1) / 2;      /* t = n(n+1)/2, triangular number g(n) */
    seq_t f_leq = (t + 2) / 3;        /* number of f(i) <= t : 3i-2 <= t */
    return tn + f_leq;                /* a(n) = n + f_leq */
}
