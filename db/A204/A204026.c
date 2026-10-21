/*
 * A204026  Symmetric matrix based on f(i,j)=min(F(i+1),F(j+1)), where
 *          F=A000045 (Fibonacci numbers), by antidiagonals.
 * https://oeis.org/A204026
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = min(F_{i+1}, F_{j+1}) for the pair (i,j) obtained by *
 *            enumerating the matrix M given by f(i,j)=min(F_{i+1},F_{j+1})
 *            along * antidiagonals. For n >= 1, let m be the smallest integer
 *            such that * n <= m(m+1)/2, let k = n - m(m-1)/2 - 1 (zero–based
 *            offset in that * antidiagonal), let i = k+1...
 * Range:     u8 n <= 96, u16 n <= 96, u32 n <= 96, u64 n <= 96, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset starts at 1, but guard */

    /* Find the antidiagonal index m with n <= m(m+1)/2 */
    idx_t m = 1;
    while ( ((__int128)m * (m + 1)) / 2 < n ) m++;

    /* Compute offset within the antidiagonal */
    idx_t prev_tri = (idx_t)(((__int128)(m - 1) * m) / 2);
    idx_t k = n - prev_tri - 1;     /* zero‑based */

    /* Corresponding matrix coordinates (1‑based) */
    idx_t i = k + 1;
    idx_t j = m - k;

    /* Fibonacci indices: need F_{i+1} and F_{j+1} */
    idx_t f1 = i + 1;
    idx_t f2 = j + 1;

    /* Static cache of Fibonacci numbers: fib[0] = 0, fib[1] = 1 */
    static seq_t fibs[300];
    static idx_t fib_len = 0;

    if (fib_len == 0) {
        fibs[0] = 0;
        fibs[1] = 1;
        fib_len = 2;
    }

    /* Extend cache if necessary */
    idx_t need = (f1 > f2) ? f1 : f2;
    for (idx_t idx = fib_len; idx <= need; idx++) {
        fibs[idx] = fibs[idx - 1] + fibs[idx - 2];
    }
    fib_len = need + 1;

    seq_t v1 = fibs[f1];
    seq_t v2 = fibs[f2];
    return (v1 < v2) ? v1 : v2;
}
