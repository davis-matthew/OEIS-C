/*
 * A213584  Rectangular array: (row n) = b**c, where b(h) = F(h+1), c(h) =
 *          n-1+h, where F=A000045 (Fibonacci numbers), n>=1, h>=1, and ** =
 *          convolution.
 * https://oeis.org/A213584
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = value of T(r,k) on the antidiagonal containing n, where
 *            T(n,k) = Fibonacci(k+4) + n*Fibonacci(k+3) – 2·(n+k) – 3. The
 *            indices n,k≥1. The mapping from the sequence index to the array
 *            coordinates is: - Let d be the smallest integer ≥2 such that
 *            (d-1)*d/2 ≥ n. - Let r = n – (d-2)*(d-1)/2 (row...
 * Range:     u8 n <= 36, u16 n <= 190, u32 n <= 903, u64 n <= 1830,
 *            u128 n <= 1830, big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/fib.h"        /* provides: seq_t fib(idx_t n); */

seq_t a(idx_t n)
{
    /* Find diagonal d containing index n.  (d-1)*d/2 counts entries
       up through diagonal d. */
    idx_t d = 1;
    while ((d - 1) * d / 2 < n)
        ++d;

    /* Number of elements before diagonal d. */
    idx_t prev = (d - 2) * (d - 1) / 2;

    /* Row (r) and column (k) of the element. */
    idx_t r = n - prev;   /* 1 ≤ r ≤ d-1 */
    idx_t k = d - r;      /* 1 ≤ k ≤ d-1 */

    /* Compute T(r,k) using the closed formula. */
    seq_t val = fib(k + 4);                    /* Fibonacci(k+4)     */
    val += (seq_t)r * fib(k + 3);              /* + r·Fibonacci(k+3) */
    val -= (seq_t)2 * ((seq_t)r + (seq_t)k);   /* – 2·(r+k)           */
    val -= (seq_t)3;                           /* – 3                 */

    return val;
}
