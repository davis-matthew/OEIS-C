/*
 * A143211  Triangle read by rows, T(n,k) = Fibonacci(n)*Fibonacci(k).
 * https://oeis.org/A143211
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = Fibonacci(r) * Fibonacci(k) where r is the smallest
 *            integer with r*(r+1)/2 ≥ n, and k = n - r*(r-1)/2 (the column in
 *            that row)
 * Range:     u8 n <= 34, u16 n <= 103, u32 n <= 324, u64 n <= 1175,
 *            u128 n <= 1275, big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"   /* provides seq_t fib(idx_t) */

seq_t a(idx_t n)
{
    if (n == 0)   /* offset is 1; 0 is undefined, return 0 */
        return 0;

    /* Find the row r such that the n‑th element lies in that row. */
    idx_t r = 1;
    idx_t cum = 1;               /* cumulative count after row r (triangular number) */
    while (cum < n) {
        r++;
        cum += r;
    }

    /* Compute the column k within the row. */
    idx_t prev = cum - r;        /* total elements before this row: r*(r-1)/2 */
    idx_t k = n - prev;          /* 1‑based column index */

    /* Compute the value: Fibonacci(r) * Fibonacci(k). */
    seq_t fr = fib(r);
    seq_t fk = fib(k);

    return fr * fk;
}
