/*
 * A094444  Triangular array T(n,k) = Fibonacci(n+4-k)*C(n,k), k=0..n, n>=0.
 * https://oeis.org/A094444
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = Fibonacci(r+4-c) * C(r,c) * where r and c are obtained
 *            from n by interpreting the sequence as * the row‑major
 *            enumeration of the triangular array * T(r,c) = Fibonacci(r+4-c)
 *            * C(r,c), r >= 0, 0 <= c <= r. * * Row index r is the largest
 *            integer such that r*(r+1)/2 <= n, * and column index c...
 * Range:     u8 n <= 22, u16 n <= 81, u32 n <= 307, u64 n <= 1189,
 *            u128 n <= 4496, big any n
 * Helpers:   binomial, fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/fib.h"
#include "../utils/binomial.h"

/* Compute the row number r for a given flat index n
 * (the largest r with r*(r+1)/2 <= n).  Uses binary search to stay
 * well within time limits even for very large n. */
static inline idx_t row_of_index(idx_t n)
{
    idx_t low  = 0;
    idx_t high = 1;

    /* Find an upper bound for r */
    while ((unsigned __int128)high * (high + 1) / 2 <= n)
        high <<= 1;

    /* Binary search for the largest r with r*(r+1)/2 <= n */
    while (low < high)
    {
        idx_t mid = low + (high - low) / 2;
        if ((unsigned __int128)mid * (mid + 1) / 2 <= n)
            low = mid + 1;
        else
            high = mid;
    }
    return low - 1;
}

seq_t a(idx_t n)
{
    /* Determine row r and column c from the flat index n. */
    idx_t r     = row_of_index(n);
    idx_t prev  = (unsigned __int128)r * (r + 1) / 2;   /* sum of rows 0..r-1 */
    idx_t c     = n - prev;                            /* column in row r */

    /* Index for Fibonacci: r + 4 - c */
    idx_t fib_idx = r + 4 - c;

    /* Retrieve values from utilities */
    seq_t fib_val = fib(fib_idx);
    seq_t binom_val = binomial(r, c);

    /* Return product */
    return fib_val * binom_val;
}
