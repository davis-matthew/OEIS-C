/*
 * A131411  Triangle read by rows: T(n,k) = Fibonacci(n) + Fibonacci(k) - 1.
 * https://oeis.org/A131411
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = fib(r) + fib(k) - 1, where r = smallest integer with
 *            r(r+1)/2 >= n, and k = n - r(r-1)/2 (1 <= k <= r)
 * Range:     u8 n <= 77, u16 n <= 298, u32 n <= 1126, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* Find the row r and column k of the nth entry of
       the triangle read by rows. */
    idx_t r = 0;
    idx_t cum = 0;             /* cumulative number of entries up to row r */
    while (cum < n) {
        ++r;
        cum += r;              /* cum = r(r+1)/2 after loop */
    }

    idx_t prev = cum - r;      /* entries before row r */
    idx_t k = n - prev;        /* 1‑based column index in row r */

    seq_t val = fib(r);
    val = val + fib(k);
    val = val - 1;
    return val;
}
