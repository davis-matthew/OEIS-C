/*
 * A136391  a(n) = n*F(n) - (n-1)*F(n-1), where the F(j)'s are the Fibonacci
 *          numbers (F(0)=0, F(1)=1).
 * https://oeis.org/A136391
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*F(n) - (n-1)*F(n-1) (Offset: 1)
 * Range:     u8 n <= 10, u16 n <= 20, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

/* Computes the nth term of OEIS sequence A136391.
 * The sequence is defined for n >= 1 by
 *   a(n) = n*F(n) - (n-1)*F(n-1)
 * where F(k) is the k-th Fibonacci number with
 *   F(0) = 0, F(1) = 1.
 *
 * This implementation calls the supplied fibonacci routine
 * `fib(idx_t n)` to obtain F(n) and F(n-1).  The result is
 * computed using the type `seq_t` so that it works for both
 * fixed‑size integer builds and arbitrary‑precision builds.
 */
seq_t a(idx_t n)
{
    /* n is guaranteed to be >= 1, so n-1 is safe. */
    seq_t fn  = fib(n);          /* F(n)  */
    seq_t fn1 = fib(n - 1);      /* F(n-1) */

    seq_t term = (seq_t)n * fn - (seq_t)(n - 1) * fn1;
    return term;
}
