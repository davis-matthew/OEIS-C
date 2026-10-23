/*
 * A191584  Diagonal sums of the Riordan matrix (1/(1-3*x^2),x/(1-x))
 *          (A191582).
 * https://oeis.org/A191584
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^(floor(n/2)+1) - fibonacci(n+3)
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   fib, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* floor(n/2) is just n >> 1 for unsigned types */
    idx_t k = (n >> 1) + (idx_t)1;          /* exponent for 3 */
    seq_t pow_val = pow3(k);                /* 3^k */
    seq_t fib_val = fib(n + (idx_t)3);      /* Fibonacci(n+3) */
    return pow_val - fib_val;
}
