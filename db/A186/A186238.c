/*
 * A186238  Adjusted joint rank sequence of (f(i)) and (g(j)) with f(i) after
 *          g(j) when f(i)=g(j), where f and g are the triangular numbers and
 *          heptagonal numbers. Complement of A186237.
 * https://oeis.org/A186238
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = rank of the nth heptagonal number in the sorted union of
 *            triangular and heptagonal numbers, with heptagonal numbers
 *            ranked before triangular numbers when equal. a(n) = n + (# of
 *            triangular numbers strictly less than H_n), where H_n =
 *            n*(5*n-3)/2
 * Range:     u8 n <= 5, u16 n <= 27, u32 n <= 96, u64 n <= 96, u128 n <= 96,
 *            big any n
 * Helpers:   triangular_count_leq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/triangular_count_leq.h"

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t five_n = (seq_t)5 * n_seq;              /* 5*n */
    seq_t H = n_seq * (five_n - 3) / 2;           /* H_n = n*(5*n-3)/2 */
    seq_t X = H - 1;                              /* triangular numbers < H_n */
    seq_t tcnt = triangular_count_leq(X);         /* count of T_i < H_n */
    return n_seq + tcnt;                          /* rank of H_n */
}
