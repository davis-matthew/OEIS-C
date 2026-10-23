/*
 * A186222  Adjusted joint rank sequence of (g(i)) and (f(j)) with f(i) after
 *          g(j) when f(i)=g(j), where f and g are the triangular numbers and
 *          squares. Complement of A186221.
 * https://oeis.org/A186222
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(-1/2 + sqrt(2*n^2))
 * Range:     u8 n <= 5, u16 n <= 90, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t t = (seq_t)2 * n_seq * n_seq;           /* 2*n^2   */
    seq_t s = floor_sqrt(t);                     /* floor(sqrt(2*n^2)) */

    seq_t L = (seq_t)8 * n_seq * n_seq;           /* 8*n^2   */
    seq_t two_s_plus_one = (seq_t)2 * s + 1;      /* 2*s+1   */
    seq_t D = two_s_plus_one * two_s_plus_one;   /* (2*s+1)^2 */

    seq_t adjust = (L < D) ? 1 : 0;               /* subtract 1 if fractional part < 0.5 */
    return n_seq + s - adjust;
}
