/*
 * A213579  Rectangular array: (row n) = b**c, where b(h) = F(h), c(h) =
 *          n-1+h, where F=A000045 (Fibonacci numbers), n>=1, h>=1, and ** =
 *          convolution.
 * https://oeis.org/A213579
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 210, u32 n <= 946, u64 n <= 1830,
 *            u128 n <= 1830, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula (OEIS A213579):
   a(n) is taken from the array T(row,column) where
   T(r,c) = Fibonacci(c+3) + r*Fibonacci(c+2) – (r + c + 2).
   The array is read by falling antidiagonals: for an index n,
   let d be the smallest integer with d*(d+1)/2 ≥ n,
   let r = n – d*(d‑1)/2 – 1 (0‑based offset within the d‑th antidiagonal),
   then row = r + 1, column = d – r.
   Finally a(n) = T(row,column). */

#include <stdint.h>

/* Computes a(n) for the OEIS sequence A213579. */
seq_t a(idx_t n)
{
    if (n < 1)          /* undefined for n < 1, but return 0 safely */
        return 0;

    /* Determine d such that d*(d+1)/2 ≥ n.  */
    idx_t d = 0;
    while ((d * (d + 1)) / 2 < n)
        ++d;

    /* Number of terms before the d‑th antidiagonal. */
    idx_t prev = (d - 1) * d / 2;

    /* r is the 0‑based position inside this diagonal. */
    idx_t r = n - prev - 1;          /* 0 ≤ r < d */

    idx_t row = r + 1;               /* 1‑based row index */
    idx_t col = d - r;               /* 1‑based column index */

    /* Compute Fibonacci values up to col+3.  */
    idx_t target = col + 3;          /* we need Fibonacci(target) and Fibonacci(target‑1) */
    seq_t f_i_minus2 = 0;            /* F(0)   */
    seq_t f_i_minus1 = 1;            /* F(1)   */
    seq_t f = 0;

    for (idx_t i = 2; i <= target; ++i) {
        f = f_i_minus1 + f_i_minus2;   /* F(i) = F(i‑1)+F(i‑2) */
        f_i_minus2 = f_i_minus1;       /* shift for next iteration */
        f_i_minus1 = f;
    }
    /* After the loop: f_i_minus1 = F(target), f_i_minus2 = F(target‑1). */
    seq_t fib_col_plus3 = f_i_minus1;
    seq_t fib_col_plus2 = f_i_minus2;

    /* Compute the term: T(row,col) = F(col+3) + row*F(col+2) – (row + col + 2). */
    seq_t term = fib_col_plus3 + (seq_t)row * fib_col_plus2
                 - ((seq_t)row + (seq_t)col + 2);

    return term;
}
