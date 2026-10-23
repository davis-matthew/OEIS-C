/*
 * A193908  Triangular array: the fusion of (p(n,x)) by (q(n,x)), where
 *          p(n,x)=sum{F(k+2)*x^(n-k) : 0<=k<=n}, where F=A000045 (Fibonacci
 *          numbers), and q(n,x)=2x*q(n-1,x)+1 with q(0,x)=1.
 * https://oeis.org/A193908
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(0) = 1. For n > 0 let r be the unique non‑negative integer
 *            with r * (r + 1) / 2 ≤ n < (r + 1) * (r + 2) / 2, and let c = n
 *            - r * (r + 1) / 2 (column index, 0‑based). Then a(n) = 2^(r‑c) *
 *            (F(r+3) – (c==r ? 2 : F(r‑c+2)) ), where F(k) is the k‑th
 *            Fibonacci number (F(0)=0, F(1)=1)
 * Range:     u8 n <= 14, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   fib, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/fib.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* handle the first term */
    if (n == 0)
        return (seq_t)1;

    /* find row r: r(r+1)/2 ≤ n < (r+1)(r+2)/2 */
    long double t = sqrtl(1.0L + 8.0L * (long double)n);
    idx_t r = (idx_t)((t - 1.0L) / 2.0L);

    /* correct possible rounding errors */
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;
    while (r * (r + 1) / 2 > n)
        --r;

    idx_t c = n - r * (r + 1) / 2;          /* column in the row */
    seq_t twopow = pow2(r - c);             /* 2^(r-c) */
    seq_t fib_root = fib(r + 3);            /* F(r+3) */

    seq_t diff;
    if (c == r) {                          /* last element in the row */
        diff = fib_root - (seq_t)2;
    } else {
        diff = fib_root - fib(r - c + 2);
    }

    return twopow * diff;
}
