/*
 * A202253  Number of zero-sum -n..n arrays of 3 elements with adjacent
 *          element differences also in -n..n.
 * https://oeis.org/A202253
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + 2*floor((2*n^2+2*n)/3) (a(n) = 1 + 2*floor(
 *            (2*n*(n+1)) / 3 ) )
 * Range:     u8 n <= 10, u16 n <= 191, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;             // cast index to seq_t
    seq_t prod = nn * (nn + 1);     // n*(n+1)
    prod = prod * 2;                // 2*n*(n+1)
    seq_t q = prod / 3;             // floor division
    return q * 2 + 1;               // final result
}
