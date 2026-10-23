/*
 * A033811  Convolution of natural numbers n >= 1 with Lucas numbers
 *          L(k)(A000032) for k >= 2.
 * https://oeis.org/A033811
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = L(5)*(F(n+1)-1) + L(4)*F(n) - L(3)*n Where L(k) are Lucas
 *            numbers and F(k) Fibonacci numbers
 * Range:     u8 n <= 6, u16 n <= 18, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* Lucas constants */
    const seq_t L5 = (seq_t)11;
    const seq_t L4 = (seq_t)7;
    const seq_t L3 = (seq_t)4;

    /* Fibonacci numbers */
    const seq_t fn  = fib(n);     /* F(n)   */
    const seq_t fn1 = fib(n + 1); /* F(n+1) */

    /* Convert index to seq_t */
    const seq_t nseq = (seq_t)n;

    /* Compute the formula */
    return L5 * (fn1 - (seq_t)1) + L4 * fn - L3 * nseq;
}
