/*
 * A169613  Triangular array: T(n,k)=floor(F(n)/F(n-k)), k=1,2,...,n-2; n>=3,
 *          where F=A000045 (Fibonacci numbers).
 * https://oeis.org/A169613
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n‑th element (n ≥ 3) obtained by row‑wise
 *            enumeration of the triangular array T(m,k) = floor(Fibonacci(m)
 *            / Fibonacci(m‑k)), k = 1..m‑2
 * Range:     u8 n <= 68, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* The sequence starts at index 3.  For n < 3 use 0 as a dummy value. */
    if (n < 3) return 0;

    /* p is the zero‑based position of n in the flattened array. */
    idx_t p = n - 3;

    /* Find the row m such that the cumulative number of terms
       up to row m exceeds p.  Rows are numbered from 3 onward,
       and row m contributes (m‑2) terms. */
    idx_t m = 3;
    idx_t cum = 0;
    while (cum + (m - 2) <= p)
    {
        cum += (m - 2);
        ++m;
    }

    /* k is the position within the found row (1‑based). */
    idx_t k = p - cum + 1;

    /* Compute the entry: floor(Fibonacci(m) / Fibonacci(m‑k)). */
    seq_t numerator   = fib(m);
    seq_t denominator = fib(m - k);
    return numerator / denominator;
}
