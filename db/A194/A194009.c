/*
 * A194009  Triangular array: the fission of (p(n,x)) by (q(n,x)), where
 *          p(n,x)=x*p(n-1,x)+n+1 with p(0,x)=1, and q(n,x)=sum{F(k+1)*x^(n-k)
 *          : 0<=k<=n}, where F=A000045 (Fibonacci numbers).
 * https://oeis.org/A194009
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (F_{k+3} - 1) * r + (F_{k+1} + k + 1), where r =
 *            floor((sqrt(8*n+1)-1)/2) is the row index, k = n - r*(r+1)/2 is
 *            the column index (0 ≤ k ≤ r), and F_m denotes the m-th Fibonacci
 *            number (F_0 = 0, F_1 = 1)
 * Range:     u8 n <= 34, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

static idx_t find_row(idx_t n) {
    idx_t lo = 0;
    idx_t hi = 1;
    /* Find an upper bound for the row */
    while ((__int128)hi * (hi + 1) / 2 <= n) {
        hi <<= 1;
    }
    /* Binary search to get the largest r with r*(r+1)/2 ≤ n */
    while (lo < hi) {
        idx_t mid = lo + (hi - lo + 1) / 2;
        if ((__int128)mid * (mid + 1) / 2 <= n) {
            lo = mid;
        } else {
            hi = mid - 1;
        }
    }
    return lo;
}

seq_t a(idx_t n) {
    /* Determine the row (r) and column (k). */
    idx_t r = find_row(n);
    idx_t tri = (idx_t)((__int128)r * (r + 1) / 2);
    idx_t k = n - tri;

    /* Fibonacci numbers needed. */
    seq_t fk1 = fib(k + 1);   /* F_{k+1} */
    seq_t fk3 = fib(k + 3);   /* F_{k+3} */

    /* Compute the term using the derived formula. */
    seq_t term = (fk3 - 1) * (seq_t)r + fk1 + (seq_t)k + 1;
    return term;
}
