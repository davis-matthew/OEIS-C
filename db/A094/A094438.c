/*
 * A094438  Triangular array T(n,k) = Fibonacci(k+3)*C(n,k), k=0..n, n>=0.
 * https://oeis.org/A094438
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = Fibonacci(col+3) * C(row, col) where row =
 *            floor((sqrt(8*n+1)-1)/2) col = n - row(row+1)/2 (This is just
 *            the triangular array read row‑by‑row.)
 * Range:     u8 n <= 30, u16 n <= 97, u32 n <= 337, u64 n <= 1202,
 *            u128 n <= 4612, big any n
 * Helpers:   binomial, fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"   /* seq_t binomial(idx_t, idx_t) */
#include "../utils/fib.h"        /* seq_t fib(idx_t) */

seq_t a(idx_t n)
{
    /* Find the row r such that r(r+1)/2 <= n < (r+1)(r+2)/2 */
    idx_t r = 0;
    /* Use a simple scan; r is O(sqrt(n)) which is trivial for n <= 10 000 */
    while (((r + 1) * (r + 2)) / 2 <= n) {
        r++;
    }

    /* Start index of this row */
    seq_t s0 = (seq_t)r * ((seq_t)(r + 1)) / 2;
    idx_t k = n - (idx_t)s0;   /* column within the row, 0 <= k <= r */

    /* Compute the value: binomial(r, k) * Fibonacci(k+3) */
    seq_t bin = binomial(r, k);
    seq_t fibk = fib(k + 3);

    return bin * fibk;
}
