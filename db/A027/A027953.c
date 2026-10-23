/*
 * A027953  a(0)=1, a(n) = Fibonacci(2n+4) - (2n+3).
 * https://oeis.org/A027953
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Fib(2n+4) - (2n+3) (a(0) = 1 is a special case)
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 21, u64 n <= 44, u128 n <= 91,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/fib.h"

/* Computes a(n) of OEIS A027953. */
seq_t a(idx_t n)
{
    /* Special case for n = 0 */
    if (n == 0)
        return (seq_t)1;

    /* Compute index for Fibonacci: 2*n + 4 */
    seq_t two_n = (seq_t)n * (seq_t)2;
    idx_t fib_index = (idx_t)(two_n + (seq_t)4);

    /* Compute Fibonacci number */
    seq_t f = fib(fib_index);

    /* Subtract (2*n + 3) */
    seq_t subtract = two_n + (seq_t)3;

    return f - subtract;
}
