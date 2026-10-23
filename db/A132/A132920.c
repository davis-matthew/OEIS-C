/*
 * A132920  a(n) = n*Fibonacci(n) + binomial(n, 2).
 * https://oeis.org/A132920
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*Fibonacci(n) + binomial(n, 2) (offset 1)
 * Range:     u8 n <= 8, u16 n <= 18, u32 n <= 40, u64 n <= 84, u128 n <= 175,
 *            big any n
 * Helpers:   binomial, fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n < 1) {
        /* a(0) is undefined for this sequence; return 0 for safety. */
        return 0;
    }

    /* Obtain F(n) and compute n choose 2. */
    seq_t fn  = fib(n);            /* Fibonacci(n)  (F(1)=1, F(2)=1, ...) */
    seq_t n_s = (seq_t)n;          /* cast index to seq_t */
    seq_t bin = binomial(n, 2);    /* n choose 2 */

    /* a(n) = n * F(n) + binomial(n, 2) */
    return n_s * fn + bin;
}
