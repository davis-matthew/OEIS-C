/*
 * A155881  a(n) is the number of zeros needed to write the integers 1 through
 *          Fibonacci(n).
 * https://oeis.org/A155881
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A061217(Fibonacci(n)) (zeros in 1..Fib(n))
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 47, u64 n <= 92, u128 n <= 183,
 *            big any n
 * Helpers:   count_zeros_upto, fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"
#include "../utils/count_zeros_upto.h"

/* Main sequence function as required by the build system. */
seq_t a(idx_t n)
{
    seq_t fn = fib(n);                     /* Fibonacci(n) */
    return count_zeros_upto(fn);           /* zeros in 1..Fib(n) */
}
