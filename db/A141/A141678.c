/*
 * A141678  Symmetrical triangle of coefficients based on invert transform of
 *          A001906.
 * https://oeis.org/A141678
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where * r = smallest integer with r*(r+1)/2 ≥ n
 *            (row number, r ≥ 1) * k = n - (r-1)*r/2 (column number in the
 *            row, 1 ≤ k ≤ r) * T(r,k) = f(r-k+1) * f(k) * with f(x) =
 *            fib(2*x), fib(0)=0, fib(1)=1 (the standard Fibonacci numbers) *
 *            * This implements the symmetric triangle of coeffici...
 * Range:     u8 n <= 21, u16 n <= 78, u32 n <= 276, u64 n <= 1081,
 *            u128 n <= 4279, big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"  /* provides seq_t fib(idx_t n) */

seq_t a(idx_t n) {
    if (n == 0) return 0;           /* undefined for n<1, return 0 as safe value */

    /* Find the row r (1‑based) that contains the nth element. */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n) {
        ++r;
    }

    /* Column index k (1‑based) within that row. */
    idx_t prev_sum = (r - 1) * r / 2;
    idx_t k = n - prev_sum;   /* 1 ≤ k ≤ r */

    /* Compute the two Fibonacci terms f(r-k+1) and f(k). */
    idx_t idx1 = r - k + 1;    /* ≥1 */
    idx_t idx2 = k;            /* ≥1 */

    seq_t f1 = fib(2 * idx1);
    seq_t f2 = fib(2 * idx2);

    return f1 * f2;
}
