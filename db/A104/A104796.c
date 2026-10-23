/*
 * A104796  Triangle read by rows: T(n,k) = (n+1-k)*Fibonacci(n+2-k), for
 *          n>=1, 1<=k<=n.
 * https://oeis.org/A104796
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (row_of(n)+1-k) * Fib(row_of(n)+2-k) where row_of(n) is
 *            the smallest r with r*(r+1)/2 >= n, k = n - (r-1)*r/2, and
 *            Fib(1)=Fib(2)=1
 * Range:     u8 n <= 28, u16 n <= 153, u32 n <= 780, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   fib_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include "../utils/fib_seq.h"

seq_t a(idx_t n) {
    /* Find the row r such that r*(r+1)/2 >= n */
    idx_t r = (idx_t)((sqrt(8.0 * (double)n + 1.0) - 1.0) / 2.0);
    while (((__int128)r * (r + 1)) / 2 < n) ++r;
    while (r > 1 && (((__int128)(r - 1) * r) / 2) >= n) --r;

    /* Compute previous cumulative count and column k */
    idx_t prev = (idx_t)(((__int128)(r - 1) * r) / 2);
    idx_t k = n - prev;                /* 1‑based column index */

    /* Compute m = r + 2 - k and the value T(r,k) */
    idx_t m = r + 2 - k;               /* Fibonacci index */
    seq_t fib = fib_seq(m);
    seq_t mult = (seq_t)(r + 1 - k);
    return mult * fib;
}
