/*
 * A128589  Triangle read by rows: T(n,k) = Fibonacci(k) * A051731(n,k).
 * https://oeis.org/A128589
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   The triangle T(n,k) equals Fibonacci(k) if k divides n,
 *            otherwise 0. a(n) is the row‑wise reading of that triangle. Thus
 *            for a given linear index n we find the row r with
 *            triangular(r-1) < n ≤ triangular(r), let k = n -
 *            triangular(r-1), and return a(n) = fib(k) if r % k == 0 else 0
 * Range:     u8 n <= 104, u16 n <= 324, u32 n <= 1175, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 >= n */
    idx_t r = 1;
    while (r * (r + 1) / 2 < n)
        ++r;

    /* Compute k = position within the row */
    idx_t prev = r * (r - 1) / 2;   /* triangular(r-1) */
    idx_t k = n - prev;

    /* Return Fibonacci(k) if k divides r, otherwise 0 */
    if (r % k == 0)
        return fib(k);  /* fib function returns seq_t */
    else
        return 0;
}
