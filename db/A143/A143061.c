/*
 * A143061  Triangle read by rows, A000012 * A127647 * A000012.
 * https://oeis.org/A143061
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r, k) where T(r,k)=Fib(r+2)-Fib(r+2-k) with * n =
 *            r*(r-1)/2 + k, 1 <= k <= r. * Implements the triangle read by
 *            rows for OEIS A143061
 * Range:     u8 n <= 68, u16 n <= 257, u32 n <= 1039, u64 n <= 4191,
 *            u128 n <= 9685, big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* Find the row r such that 1+2+…+r >= n */
    idx_t r = 1;
    idx_t cumulative = 1;
    while (cumulative < n) {
        ++r;
        cumulative += r;
    }

    /* Position k within that row (1‑based) */
    idx_t prev = r * (r - 1) / 2;
    idx_t k = n - prev;

    /* Compute T(r,k) = Fib(r+2) - Fib(r+2-k) */
    seq_t fib_r_plus_2          = fib(r + 2);
    seq_t fib_r_plus_2_minus_k = fib(r + 2 - k);

    return fib_r_plus_2 - fib_r_plus_2_minus_k;
}
