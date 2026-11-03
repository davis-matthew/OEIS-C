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
 * Validated: partial, pass, 96 terms through n = 96 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Return the count of triangular numbers ≤ X (i.e. k*(k+1)/2 ≤ X). */
static seq_t triangular_count_leq(seq_t X)
{
    seq_t low = 0;
    seq_t high = X;
    while (low < high) {
        seq_t mid = (low + high + 1) / 2;          /* ceil((low+high)/2) */
        seq_t tri = mid * (mid + 1) / 2;           /* triangular(mid) */
        if (tri <= X)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t five_n = (seq_t)5 * n_seq;              /* 5*n */
    seq_t H = n_seq * (five_n - 3) / 2;           /* H_n = n*(5*n-3)/2 */
    seq_t X = H - 1;                              /* triangular numbers < H_n */
    seq_t tcnt = triangular_count_leq(X);         /* count of T_i < H_n */
    return n_seq + tcnt;                          /* rank of H_n */
}
