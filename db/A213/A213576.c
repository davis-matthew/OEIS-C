/*
 * A213576  Rectangular array: (row n) = b**c, where b(h) = h, c(h) =
 *          F(n-1+h), where F=A000045 (Fibonacci numbers), n >= 1, h >= 1, and
 *          ** = convolution.
 * https://oeis.org/A213576
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(r,c) where \ T(r,c) = F(r+c+3) – c*F(r+1) – F(r+3), \
 *            with (r,c) obtained by antidiagonal enumeration: for n ≥ 1, let
 *            s be the smallest integer ≥ 2 such that s*(s-1)/2 ≥ n; let t = n
 *            – (s-2)*(s-1)/2; then r = t and c = s – t
 * Range:     u8 n <= 45, u16 n <= 210, u32 n <= 253, u64 n <= 253,
 *            u128 n <= 253, big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"   /* provides: seq_t fib(idx_t n) */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;     /* safety; offset is 1 */

    /* find s such that s*(s-1)/2 >= n (s starts at 2) */
    idx_t s = 2;
    while ( (__uint128_t)(s - 1) * s / 2 < n )
        ++s;

    idx_t prev = (__uint128_t)(s - 2) * (s - 1) / 2;
    idx_t t    = n - prev;          /* 1 ≤ t ≤ s-1 */
    idx_t r    = t;                 /* row index  (1‑based) */
    idx_t c    = s - t;             /* column index (1‑based) */

    /* Special cases r = 1 or 2 (avoid large product) */
    if (r == 1) {
        seq_t f = fib(c + 4);              /* F(c+4) */
        seq_t val = f - (seq_t)c - (seq_t)3;
        return val;
    }
    if (r == 2) {
        seq_t f = fib(c + 5);              /* F(c+5) */
        seq_t val = f - (seq_t)(2 * c) - (seq_t)5;
        return val;
    }

    /* r ≥ 3 : use column recurrence T(r,c) = T(r-1,c) + T(r-2,c) */
    seq_t T1 = fib(c + 4) - (seq_t)c - (seq_t)3;          /* T(1,c) */
    seq_t T2 = fib(c + 5) - (seq_t)(2 * c) - (seq_t)5;    /* T(2,c) */
    seq_t prev2 = T1;          /* T(i-2,c) */
    seq_t prev1 = T2;          /* T(i-1,c) */
    seq_t Ti;

    for (idx_t i = 3; i <= r; ++i) {
        Ti = prev1 + prev2;
        prev2 = prev1;
        prev1 = Ti;
    }

    return Ti;
}
