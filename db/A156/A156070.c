/*
 * A156070  Triangle read by rows based on the Fibonacci sequence A000045:
 *          t(n,m) = 1 + Fibonacci[n] - Fibonacci[m] - Fibonacci[n - m].
 * https://oeis.org/A156070
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 1 + Fibonacci[r] – Fibonacci[c] – Fibonacci[r‑c] , *
 *            where (r,c) is obtained from the triangular decomposition of n:
 *            * r is the largest integer such that r(r+1)/2 ≤ n, * c = n –
 *            r(r+1)/2 . * * The implementation uses an iterative Fibonacci
 *            routine that matches * the standard 0‑indexed Fibo...
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   fib_u
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/fib_u.h"

seq_t a(idx_t n)
{
    /* Determine the row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2. */
    idx_t r = 0;
    while ((idx_t)(r + 1) * (r + 2) / 2 <= n) {
        r++;
    }

    /* Column index c within that row. */
    idx_t prefix = r * (r + 1) / 2;
    idx_t c = n - prefix;

    /* Compute the required Fibonacci values. */
    seq_t fr = fib_u(r);
    seq_t fc = fib_u(c);
    seq_t frc = fib_u(r - c);

    /* According to the definition, the result is non‑negative. */
    return (seq_t)1 + fr - fc - frc;
}
