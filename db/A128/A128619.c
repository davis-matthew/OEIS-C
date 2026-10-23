/*
 * A128619  Triangle T(n, k) = A127647(n,k) * A128174(n,k), read by rows.
 * https://oeis.org/A128619
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the sequence obtained by reading the triangle T(r,c) =
 *            Fibonacci(r) * (1 + (-1)^(r+c)) / 2 by rows. That is, a(n) =
 *            Fibonacci(row) if (row + col) is even, otherwise 0
 * Range:     u8 n <= 92, u16 n <= 300, u32 n <= 1129, u64 n <= 4372,
 *            u128 n <= 5050, big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* offset starts at 1 */

    /* Find the row r such that r(r+1)/2 >= n */
    idx_t r = 1;
    idx_t cumulative = 1;          /* sum of lengths of rows up to r */
    while (cumulative < n) {
        ++r;
        cumulative += r;
    }

    /* Column index within the row: 1 ≤ col ≤ r */
    idx_t col = n - (cumulative - r);

    /* If r + col is odd, the triangle entry is zero */
    if ( ((r + col) & 1) != 0 )
        return 0;

    /* Otherwise the entry equals Fibonacci(r) */
    return fib(r);
}
