/*
 * A135992  Positive Fibonacci numbers swapped in pairs.
 * https://oeis.org/A135992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (Lucas(n) - (-1)^n * Fibonacci(n)) / 2
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Lucas(n) = Fibonacci(n-1) + Fibonacci(n+1) */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* Compute Fibonacci terms needed */
    seq_t fn    = fib(n);
    seq_t fn_1  = fib(n - 1);   /* valid for n >= 1 */
    seq_t fn_1p = fib(n + 1);

    /* Lucas(n) = F(n-1) + F(n+1) */
    seq_t lucas = fn_1 + fn_1p;

    /* (-1)^n is +1 for even n, -1 for odd n */
    if ((n & 1) == 0) {           /* even n */
        return (lucas - fn) / 2;
    } else {                      /* odd n */
        return (lucas + fn) / 2;
    }
}
