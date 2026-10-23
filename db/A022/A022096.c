/*
 * A022096  Fibonacci sequence beginning 1, 6.
 * https://oeis.org/A022096
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*Fib(n+2) - 4*Fib(n+1) (for n ≥ 0)
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 43, u64 n <= 89, u128 n <= 182,
 *            big any n
 * Helpers:   fib, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* from the entry: a(n) = a(n‑1) + a(n‑2) with a(0)=1, a(1)=6 */

#include <stdlib.h>
#include "../utils/fib.h"   /* provides seq_t fib(idx_t n);          */
#include "../utils/seq_mul.h" /* provides seq_mul(seq_t x, unsigned int m); */

seq_t
a(idx_t n)
{
    /* Handle the two fixed initial values */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)6;

    /* Compute Fibonacci numbers needed (n+2) and (n+1) */
    seq_t fn2 = fib(n + 2);
    seq_t fn1 = fib(n + 1);

    /* a(n) = 5*fn2 - 4*fn1 */
    seq_t term5 = seq_mul(fn2, 5u);   /* 5 * Fib(n+2) */
    seq_t term4 = seq_mul(fn1, 4u);   /* 4 * Fib(n+1) */

    return term5 - term4;
}
