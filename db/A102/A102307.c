/*
 * A102307  a(n) = Fibonacci(2n+1) * binomial(2n,n).
 * https://oeis.org/A102307
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Fibonacci(2n+1) * binomial(2n,n)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Validated: partial, pass, 22 terms through n = 21 (2026-11-01)
 * Helpers:   binomial, fibonacci
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <sys/types.h>
#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/fibonacci.h"

seq_t a(idx_t n)
{
    /* compute the two factors separately */
    seq_t fib_val = fibonacci(2*n + 1);   /* Fibonacci(2n+1)   */
    seq_t bin_val = binomial(2*n, n);      /* binomial(2n,n)     */

    /* multiply to obtain the sequence term */
    return fib_val * bin_val;
}
