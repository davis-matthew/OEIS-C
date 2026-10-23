/*
 * A111946  Triangle read by rows: T(n,k) = gcd(Fibonacci(n), Fibonacci(k)), 1
 *          <= k <= n.
 * https://oeis.org/A111946
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = Fibonacci(gcd(r, k)) where r is the row index of the nth
 *            * triangle entry (the smallest r with r(r+1)/2 ≥ n) and k = n -
 *            (r-1)r/2 * is the column index within that row. The triangle
 *            entry T(r,k) = * gcd(Fibonacci(r), Fibonacci(k)) =
 *            Fibonacci(gcd(r,k))
 * Range:     u8 n <= 104, u16 n <= 324, u32 n <= 1175, u64 n <= 4464,
 *            u128 n <= 9014, big any n
 * Helpers:   fib_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/fib_u64.h"

seq_t a(idx_t n)
{
    /* Find row r such that r(r+1)/2 ≥ n.  r starts at 1. */
    idx_t r = 1;
    for (;;) {
        unsigned __int128 cum = (unsigned __int128)r * (r + 1) / 2;
        if (cum >= n)
            break;
        ++r;
    }

    /* Column index k within the row: 1 ≤ k ≤ r. */
    unsigned __int128 prev = (unsigned __int128)(r - 1) * r / 2;
    idx_t k = n - prev;

    /* Compute gcd(r, k) using Euclid's algorithm. */
    idx_t a_val = r, b_val = k;
    while (b_val != 0) {
        idx_t t = a_val % b_val;
        a_val = b_val;
        b_val = t;
    }
    idx_t g = a_val;          /* g = gcd(r, k) */

    /* Return Fibonacci(g).  fib_u64 returns a seq_t. */
    return fib_u64(g);
}
