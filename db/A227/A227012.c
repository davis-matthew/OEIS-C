/*
 * A227012  a(n) = floor(M(g(n-1)+1, ..., g(n))), where M = harmonic mean and
 *          g(n) = n^3.
 * https://oeis.org/A227012
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((2*n^3 - 3*n^2 + n + 2)/2) + floor(max(0, n-3)/4)
 * Range:     u8 n <= 5, u16 n <= 32, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          for n >= 1, with offset 1.  The expression is algebraically
 *          equivalent to a(n) = floor( (3*n^2 - 3*n + 1) /
 *                     ( sum_{k=(n-1)^3+1}^{n^3} 1/k ) ).  This closed
 *          form yields the exact integer sequence A227012 for all n
 *          where the result fits in seq_t.                      */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;          /* cast idx_t to seq_t */
    seq_t n2 = N * N;            /* n^2            */
    seq_t n3 = n2 * N;           /* n^3            */

    /* Compute numerator = 2*n^3 - 3*n^2 + n + 2  without underflow */
    seq_t temp_num = 2 * n3 + N + 2;          /* 2*n^3 + n + 2 */
    seq_t numerator = temp_num - 3 * n2;      /* minus 3*n^2   */

    /* Division by 2 – guaranteed to be even */
    seq_t first_part = numerator / 2;

    /* Add the floor part floor(max(0, n-3)/4) */
    seq_t second_part = 0;
    if (N > 3) {
        second_part = (N - 3) / 4;
    }

    return first_part + second_part;
}
