/*
 * A213768  Rectangular array: (row n) = b**c, where b(h) = F(h), c(h) =
 *          2*n-3+2*h, F=A000045 (Fibonacci numbers), n>=1, h>=1, and ** =
 *          convolution.
 * https://oeis.org/A213768
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is read by falling antidiagonals from the array T defined
 *            by T(n,k) = 2*n*Fibonacci(k+2) + Lucas(k+2) - 2*(k+n) - 3. Here
 *            n,k = row,column with 1 ≤ n,k
 * Range:     u8 n <= 36, u16 n <= 190, u32 n <= 903, u64 n <= 3240,
 *            u128 n <= 3240, big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Use the provided Fibonacci routine from utils. */
#include <stddef.h>
#include "../utils/fib.h"

/* Compute a(n) for n >= 1. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;   /* offset is 1; undefined for 0 */

    /* Find diagonal d such that d(d+1)/2 >= n. */
    idx_t d = 1;
    idx_t tri = 1;                  /* triangle number for current d: d(d+1)/2 */
    while (tri < n) {
        ++d;
        tri += d;                   /* tri = d(d+1)/2 */
    }

    /* Position inside diagonal: 1 <= pos <= d. */
    idx_t tri_before = tri - d;     /* (d-1)d/2 */
    idx_t pos = n - tri_before;     /* pos = n - (d-1)d/2 */

    /* Map to row (i) and column (j) of the array. */
    idx_t i = pos;                  /* row index */
    idx_t j = d - pos + 1;          /* column index */

    /* Compute the value using the closed form. */
    seq_t f_j2 = fib(j + (idx_t)2); /* Fibonacci(j+2)   */
    seq_t f_j1 = fib(j + (idx_t)1); /* Fibonacci(j+1)   */
    seq_t f_j3 = fib(j + (idx_t)3); /* Fibonacci(j+3)   */
    seq_t lucas = f_j1 + f_j3;      /* Lucas(j+2) = F(j+1)+F(j+3) */

    seq_t term = (seq_t)2 * (seq_t)i * f_j2; /* 2*n*F(j+2) with n=i */
    term += lucas;                          /* + Lucas(j+2) */
    term -= (seq_t)2 * ((seq_t)i + (seq_t)j);
    term -= (seq_t)3;

    return term;
}
