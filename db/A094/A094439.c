/*
 * A094439  Triangular array T(n,k) = Fibonacci(k+4)*C(n,k), k=0..n, n>=0.
 * https://oeis.org/A094439
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 84, u32 n <= 313, u64 n <= 1201,
 *            u128 n <= 4518, big any n
 * Validated: standard, pass, 82 terms through n = 4542 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A094439:  a(n) = fib(k+4) * binom(r,k)
   where (r,k) are the row and column indices of the linear index n
   in the triangular array T(r,k) = fib(k+4) * C(r,k).  The array is
   flattened in row‑major order (first row  r=0, then r=1, …). */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/binomial.h"

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

static void rowcol(idx_t n, idx_t *r, idx_t *k)
{
    idx_t tri = 0;   /* total number of terms before the current row */
    idx_t row = 0;   /* current row index                              */
    while (n >= tri + row + 1) {
        tri += row + 1;  /* add the number of terms in the previous row   */
        ++row;           /* move to the next row                          */
    }
    *r = row;
    *k = n - tri;        /* column index within the row                   */
}

seq_t a(idx_t n)
{
    idx_t r, k;
    rowcol(n, &r, &k);

    seq_t f = fib(k + 4);      /* Fibonacci number F_{k+4} */
    seq_t c = binomial(r, k);     /* binomial coefficient C(r,k) */

    return f * c;
}
