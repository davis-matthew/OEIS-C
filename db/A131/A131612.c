/*
 * A131612  (Fibonacci(n)^(n+1)-1)/(Fibonacci(n)-1).
 * https://oeis.org/A131612
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (Fibonacci(n)^(n+1)-1)/(Fibonacci(n)-1) * (= 1 + F(n) +
 *            F(n)^2 + … + F(n)^n for F(n) ≠ 1; * for Fibonacci(n) = 1 the
 *            value is n+1
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Helpers:   fib, seq_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"
#include "../utils/seq_one.h"

/* Return the n-th term of OEIS A131612.  The index n starts at 0. */
seq_t a(idx_t n)
{
    /* Fibonacci value for the given index. */
    seq_t fn = fib(n);

    /* If Fibonacci(n) == 1, the original formula has a 0/0 situation.
       In that case the value equals n+1 (the limit of the geometric sum). */
    if (fn == (seq_t)1) {
        return (seq_t)(n + 1);
    }

    /* Compute the geometric series sum: 1 + fn + fn^2 + … + fn^n */
    seq_t sum  = seq_one();   /* the term for exponent 0 */
    seq_t term = seq_one();   /* current power of fn (starts as fn^0) */

    for (idx_t i = 1; i <= n; ++i) {
        term = term * fn;    /* term = fn^i */
        sum += term;        /* accumulate the series */
    }

    return sum;
}
