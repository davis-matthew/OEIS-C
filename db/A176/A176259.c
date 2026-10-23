/*
 * A176259  Triangle, T(n, k) = Fibonacci(k+1) + Fibonacci(n-k+1) -
 *          Fibonacci(n+1), read by rows.
 * https://oeis.org/A176259
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   T(n, k) = Fibonacci(k+1) + Fibonacci(n-k+1) - Fibonacci(n+1)
 *            a(n) is the linearisation of this triangle by rows, offset 0
 * Range:     i8 n <= 79, i16 n <= 278, i32 n <= 1083, i64 n <= 4280,
 *            i128 n <= 5150, big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/fib.h"

/* Find the row index r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
static inline idx_t row_for_index(idx_t n)
{
    idx_t low = 0, high = 1;
    /* Grow upper bound until it exceeds the desired row. */
    while ((unsigned __int128)high * (high + 1) / 2 <= n) {
        high <<= 1;
    }
    /* Binary search between low and high. */
    while (low + 1 < high) {
        idx_t mid = low + ((high - low) >> 1);
        if ((unsigned __int128)mid * (mid + 1) / 2 <= n) {
            low = mid;
        } else {
            high = mid;
        }
    }
    return low;
}

seq_t a(idx_t n)
{
    /* Determine row (r) and column (c) from the flattened index n. */
    idx_t row = row_for_index(n);
    idx_t tri = (idx_t)((unsigned __int128)row * (row + 1) / 2);
    idx_t col = n - tri;

    /* Compute the triangle entry using the closed‑form formula. */
    seq_t val = fib(col + 1) + fib(row - col + 1) - fib(row + 1);
    return val;
}
