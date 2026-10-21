/*
 * A057861  floor[2^n/Fibonacci(n)].
 * https://oeis.org/A057861
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(2^n / Fibonacci(n)) (offset 1)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   fib_pair, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib_pair.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Handle n < 1 defensively – behavior below the offset is irrelevant. */
    if (n == 0)
        return 0;

    /* Compute 2^n */
    seq_t pow2n = pow2(n);

    /* Retrieve Fibonacci(n) */
    seq_t fn, fn1;
    fib_pair(n, &fn, &fn1);   /* fn holds F(n), fn1 holds F(n+1) */

    /* Floor division of 2^n by F(n) is just integer division */
    return pow2n / fn;
}
