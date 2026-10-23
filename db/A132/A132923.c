/*
 * A132923  Triangle read by rows: T(n, k) = Fibonacci(k) + n - k.
 * https://oeis.org/A132923
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r, k) with T(r,k) = Fibonacci(k) + r - k, r = smallest
 *            integer such that r(r+1)/2 ≥ n, k = n - (r-1)r/2. The triangle
 *            is read row by row
 * Range:     u8 n <= 104, u16 n <= 324, u32 n <= 1175, u64 n <= 4464,
 *            u128 n <= 9462, big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses the provided fib(idx_t n) utility. */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* find the row r containing the nth term */
    idx_t r = 1;
    while (r * (r + 1) / 2 < n)
        ++r;

    /* number of elements before this row */
    idx_t before = (r - 1) * r / 2;

    /* column index within the row (1‑based) */
    idx_t k = n - before;

    /* compute the value: Fibonacci(k) + r - k */
    seq_t fk = fib(k);
    return fk + (seq_t)(r - k);
}
