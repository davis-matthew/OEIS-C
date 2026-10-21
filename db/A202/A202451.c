/*
 * A202451  Upper triangular Fibonacci matrix, by SW antidiagonals.
 * https://oeis.org/A202451
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   The sequence is obtained by enumerating the entries of the
 *            infinite upper‑triangular matrix M(i,j) = 0 for j < i =
 *            Fib(j-i+1) for j ≥ i in SW antidiagonals (constant i+j) and
 *            listing each antidiagonal from the bottom element up. For the
 *            nth term a(n) we - find the antidiagonal s with cumulative s...
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* Protect against an illegal, zero index (not part of OEIS). */
    if (n == 0)
        return 0;

    /* Find the smallest k such that T_k = k*(k+1)/2 ≥ n.
       Then s = k + 1 is the antidiagonal number, having k elements. */
    idx_t k = 1;
    while (k * (k + 1) / 2 < n)
        ++k;

    /* Position (offset) of n inside the antidiagonal (1‑based). */
    idx_t t_prev = (k - 1) * k / 2;        /* T_{k-1} */
    idx_t offset = n - t_prev;

    idx_t s = k + 1;                       /* current antidiagonal index */

    /* If the offset falls in the lower (zero) part, return 0. */
    if (offset <= (s - 1) / 2)
        return 0;

    /* Otherwise compute the Fibonacci index. */
    idx_t fib_index = 2 * offset - s + 1;   /* 1‑based Fibonacci index */

    return fib(fib_index);
}
