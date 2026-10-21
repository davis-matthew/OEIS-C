/*
 * A094441  Triangular array T(n,k) = Fibonacci(n+1-k)*C(n,k), 0 <= k <= n.
 * https://oeis.org/A094441
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = fib(m + 1 - k) * binomial(m, k), where m is the largest
 *            integer satisfying m*(m + 1)/2 <= n, and k = n - m*(m + 1)/2.
 *            This enumerates the triangular array T(m,k) = Fibonacci(m + 1 -
 *            k) * C(m,k) row by row (row 0 first)
 * Range:     u8 n <= 37, u16 n <= 108, u32 n <= 357, u64 n <= 1240,
 *            u128 n <= 4686, big any n
 * Helpers:   binomial, fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find the row m using a simple upward loop with 128‑bit guards
       to avoid overflow for very large n. */
    idx_t m = 0;
    while ( (unsigned __int128)(m + 1) * (m + 2) / 2 <= n )
        ++m;

    /* Number of elements before row m */
    unsigned __int128 before = (unsigned __int128)m * (m + 1) / 2;
    idx_t k = n - (idx_t)before;   /* column index in the row */

    /* Compute Fibonacci(m + 1 - k) * C(m, k) */
    seq_t fib_val   = fib(m + 1 - k);
    seq_t bin_val   = binomial(m, k);
    return fib_val * bin_val;
}
