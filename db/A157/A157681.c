/*
 * A157681  Fibonacci sequence beginning 29, 31.
 * https://oeis.org/A157681
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 27*Fibonacci(n) + 2*Fibonacci(n+1)
 * Range:     u8 n <= 6, u16 n <= 17, u32 n <= 40, u64 n <= 86, u128 n <= 178,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

/* Returns the n-th term of OEIS A157681 (1‑based index). */
seq_t a(idx_t n)
{
    /* The helper fib provides 0‑based Fibonacci numbers
       (fib(0)=0, fib(1)=1).  Our sequence is also 1‑based. */
    seq_t fn  = fib(n);
    seq_t fn1 = fib(n + 1);
    return ((seq_t)27 * fn) + ((seq_t)2 * fn1);
}
