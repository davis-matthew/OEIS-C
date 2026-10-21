/*
 * A094439  Triangular array T(n,k) = Fibonacci(k+4)*C(n,k), k=0..n, n>=0.
 * https://oeis.org/A094439
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 84, u32 n <= 313, u64 n <= 1201,
 *            u128 n <= 4518, big any n
 * Helpers:   binom, fib, rowcol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A094439:  a(n) = fib(k+4) * binom(r,k)
   where (r,k) are the row and column indices of the linear index n
   in the triangular array T(r,k) = fib(k+4) * C(r,k).  The array is
   flattened in row‑major order (first row  r=0, then r=1, …). */
#include <stdbool.h>
#include "../utils/fib.h"     /* provides seq_t fib(idx_t n)          */
#include "../utils/binom.h"   /* provides seq_t binom(idx_t n,idx_t k) */
#include "../utils/rowcol.h"

seq_t a(idx_t n)
{
    idx_t r, k;
    rowcol(n, &r, &k);

    seq_t f = fib(k + 4);      /* Fibonacci number F_{k+4} */
    seq_t c = binom(r, k);     /* binomial coefficient C(r,k) */

    return f * c;
}
