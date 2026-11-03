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
 *            u128 n <= 10011, big any n
 * Validated: standard, pass, 81 terms through n = 9685 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static seq_t fib(idx_t n) {
    if (n <= 2) return (seq_t)1;
    seq_t f1 = 1, f2 = 1, f3;
    for (idx_t i = 3; i <= n; ++i) {
        f3 = f1 + f2;
        f1 = f2;
        f2 = f3;
    }
    return f2;
}

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
