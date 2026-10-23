/*
 * A027942  a(n) = T(2n+1, n+2), T given by A027935.
 * https://oeis.org/A027942
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(2*n+5) - 2*n^2 - 5*n - 5
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 21, u64 n <= 44, u128 n <= 90,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* convert index to seq_t */
    seq_t n_seq = (seq_t)n;

    /* compute Fibonacci(2*n+5) */
    idx_t k = 2 * n + 5;
    seq_t fib_val = fib(k);

    /* compute subtrahend: 2 * n^2 + 5 * n + 5 */
    seq_t sub = 2 * n_seq * n_seq + 5 * n_seq + 5;

    /* final result */
    return fib_val - sub;
}
