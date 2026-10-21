/*
 * A202874  Symmetric matrix based on (1,2,3,5,8,13,...), by antidiagonals.
 * https://oeis.org/A202874
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = Σ_{k=0}^{min(i,j)} F_{i-k+2}·F_{j-k+2} * where (i,j) are
 *            obtained from the antidiagonal traversal: * let d = largest
 *            integer with (d+1)(d+2)/2 < n * prev = d(d+1)/2 * i = n - prev -
 *            1 * j = d - i * F(m) is the Fibonacci number with F(0)=0,
 *            F(1)=1. * The sequence s(t)=F(t+2) starts 1,2,3,5,
 * Range:     u8 n <= 58, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/fib.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1 */

    /* find diagonal d: minimal d with (d+1)(d+2)/2 >= n */
    idx_t d = 0;
    while ( (( (__int128)(d + 1) * (d + 2)) / 2 ) < n )
        ++d;

    idx_t prev  = d * (d + 1) / 2;          /* elements before this diagonal */
    idx_t offset= n - prev - 1;             /* position inside diagonal */
    idx_t i = offset;                       /* row index (0‑based) */
    idx_t j = d - offset;                   /* column index (0‑based) */

    idx_t limit = (i < j) ? i : j;           /* k goes from 0 to min(i,j) */

    seq_t sum = 0;
    for (idx_t k = 0; k <= limit; ++k) {
        seq_t a_val = fib(i - k + 2);       /* s[i-k] */
        seq_t b_val = fib(j - k + 2);       /* s[j-k] */
        sum += a_val * b_val;
    }
    return sum;
}
