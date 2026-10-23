/*
 * A119601  a(n) = 4*Product_{i=1..2n-1} (3^i+1)^2.
 * https://oeis.org/A119601
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*Product_{i=1..2n-1} (3^i+1)^2
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* a(0) is the empty product */
    if (n == 0)
        return (seq_t)4;

    /* upper limit for i (exclusive): i runs from 1 to 2*n-1 inclusive */
    idx_t limit = 2 * n;     /* n ≥ 1, so limit ≥ 2 */

    seq_t prod = (seq_t)1;   /* accumulating product of (3^i+1)^2 */
    seq_t pow3 = (seq_t)3;   /* current 3^i, starts at 3^1 */

    for (idx_t i = 1; i < limit; ++i) {
        seq_t term = pow3 + (seq_t)1;   /* 3^i + 1 */
        prod = prod * term * term;      /* multiply square of term */
        pow3 = pow3 * (seq_t)3;         /* advance to 3^(i+1) */
    }

    return prod * (seq_t)4;   /* final factor 4 */
}
