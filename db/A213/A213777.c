/*
 * A213777  Rectangular array: (row n) = b**c, where b(h) = F(h), c(h) =
 *          F(h+1), F=A000045 (Fibonacci numbers), n>=1, h>=1, and ** =
 *          convolution.
 * https://oeis.org/A213777
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 171, u32 n <= 780, u64 n <= 3240,
 *            u128 n <= 3240, big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A213777: read by falling antidiagonals from the rectangular array
 * T(n,k) = (k*Lucas(n+k+1) + Lucas(n)*Fibonacci(k))/5 ,
 * where Lucas(m) = Fibonacci(m-1)+Fibonacci(m+1).
 * Formula is used directly; r==1 case for Lucas(n) handled explicitly
 * because fib(0) may not be defined by the included fib routine.
 */

#include "../utils/fib.h"

/* Compute a(n) for any n >= 1 */
seq_t a(idx_t n)
{
    if (n == 0)           /* sequence is 1‑based; ignore 0 */
        return 0;

    /* Find the diagonal d that contains the index n. */
    idx_t d   = 1;           /* current diagonal (has d elements) */
    idx_t cum = 1;           /* cumulative count up to this diagonal */
    while (cum < n) {
        ++d;
        cum += d;
    }

    /* Number of elements before the current diagonal. */
    idx_t prev = cum - d;
    /* Offset of n within its diagonal (1‑based). */
    idx_t r = n - prev;               /* column index in the row   */
    idx_t c = d - r + 1;              /* row index in the column   */

    /* Compute Fibonacci numbers needed. */
    seq_t lucas_r;
    if (r == 1) {
        /* Lucas(1) = 1 by definition. */
        lucas_r = 1;
    } else {
        /* Lucas(r) = Fibonacci(r-1) + Fibonacci(r+1) */
        lucas_r = fib(r - 1) + fib(r + 1);
    }

    seq_t fib_c = fib(c);               /* Fibonacci(c) */

    /* Lucas(r + c + 1) = Lucas(d + 2) = F(d+1) + F(d+3) */
    seq_t lucas_sum = fib(d + 1) + fib(d + 3);

    /* Evaluate the formula: (c*Lucas(r+c+1) + Lucas(r)*Fibonacci(k)) / 5 */
    seq_t numerator  = c * lucas_sum + lucas_r * fib_c;
    seq_t value      = numerator / 5;

    return value;
}
