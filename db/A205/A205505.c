/*
 * A205505  a(n) = Fibonacci(n*(n+1)) / Fibonacci(n).
 * https://oeis.org/A205505
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(n*(n+1)) / Fibonacci(n)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

/* Computes A205505 term a(n) for n >= 1.
   Uses the utility function fib(idx_t) which returns Fibonacci(n) in
   the type seq_t.  The division is exact by definition of the sequence. */
seq_t a(idx_t n)
{
    /*  n*(n+1) fits safely into idx_t for all reasonable n (e.g. n <= 10000)   */
    idx_t numer_idx = (idx_t)n * (n + 1);

    seq_t fn  = fib(n);          /* Fibonacci(n)  */
    seq_t fN  = fib(numer_idx);  /* Fibonacci(n*(n+1)) */

    return fN / fn;  /* exact division */
}
