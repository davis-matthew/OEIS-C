/*
 * A169616  Triangular array: T(n,k)=integer nearest F(n+k)/F(k),
 *          k=1,2,...,n-2; n>=3, where F=A000045 (Fibonacci numbers).
 * https://oeis.org/A169616
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = nearest integer to F(r+2)/F(k+1), where r is the unique
 *            row with r*(r+1)/2 ≥ n and k = n – (r-1)*r/2 (1‑based column
 *            index)
 * Range:     u8 n <= 66, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Validated: partial, pass, 91 terms through n = 91 (2026-11-01)
 * Helpers:   isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/isqrt.h"

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
    if (n == 0) return 0;               /* n is always ≥ 1 by the offset */

    /* Find row r such that r*(r+1)/2 ≥ n. */
    idx_t r = (isqrt(1 + 8 * n) - 1) / 2;
    idx_t tri = r * (r + 1) / 2;
    while (tri < n) {
        ++r;
        tri = r * (r + 1) / 2;
    }

    /* Column index k within the row (1‑based). */
    idx_t prev_tri = (r - 1) * r / 2;
    idx_t k = n - prev_tri;

    /* Compute the nearest integer to F(r+2)/F(k+1). */
    seq_t numerator   = fib(r + 2);      /* F(r+2) */
    seq_t denominator = fib(k + 1);      /* F(k+1) */
    seq_t half_den    = denominator / 2; /* floor(denominator/2) */
    seq_t result      = (numerator + half_den) / denominator;

    return result;
}
