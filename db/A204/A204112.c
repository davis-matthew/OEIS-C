/*
 * A204112  Symmetric matrix based on f(i,j) = gcd(F(i+1), F(j+1)), where
 *          F=A000045 (Fibonacci numbers), by antidiagonals.
 * https://oeis.org/A204112
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A204112: a(n) = Fib( gcd( i+1, j+1 ) ) where (i,j) are the
 * coordinates of the n‑th entry read antidiagonally from the matrix
 * f(i,j) = gcd( Fib(i+1), Fib(j+1) ).  The Fibonacci function
 * fib() given in utils/fib.h returns the required value.  See
 * OEIS for details. */
#include <stdint.h>
#include "../utils/fib.h"

/* Euclidean algorithm for idx_t values.  Values are small (≤ 150). */
static idx_t gcd_uint(idx_t a, idx_t b)
{
    while (b != 0)
    {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* Find the diagonal s (s = i + j) containing the n-th element. */
    idx_t low = 2;
    idx_t high = 2 * n;          /* safe upper bound */
    idx_t s = high;

    while (low <= high)
    {
        idx_t mid = low + (high - low) / 2;
        __int128 cnt = (__int128)(mid - 1) * mid / 2;  /* cumulative up to mid */
        if (cnt >= n)
        {
            s = mid;
            if (mid == 0) break; /* safety for extreme n */
            high = mid - 1;
        }
        else
        {
            low = mid + 1;
        }
    }

    /* Number of entries before diagonal s */
    __int128 prev_cnt = (__int128)(s - 2) * (s - 1) / 2;
    idx_t t = n - (idx_t)prev_cnt;          /* 1‑based index within diagonal s */

    /* Map to matrix coordinates (i, j), 1‑based */
    idx_t i = t;            /* i = 1 … s-1 */
    idx_t j = s - t;        /* j = 1 … s-1 */

    /* Compute the required Fibonacci index = gcd(i+1, j+1) */
    idx_t g = gcd_uint(i + 1, j + 1);

    return fib(g);
}
