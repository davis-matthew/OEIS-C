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
 * Validated: partial, pass, 16 terms through n = 15 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

static seq_t fib(idx_t n) {
    if (n <= 2) return (seq_t)1;
    seq_t f1 = 1, f2 = 1, f3;
    for (idx_t i = 3; i <= n; ++i) {
        f3 = f1 + f2;
        f1 = f2;
        f2 = f3;
    }
    return f2;
}

static inline seq_t seq_one(void) { return (seq_t)1; }

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
