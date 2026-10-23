/*
 * A180663  Mirror image of the Golden Triangle: T(n,k) = A001654(n-k) for
 *          n>=0 and 0<=k<=n.
 * https://oeis.org/A180663
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = Fibonacci(m) * Fibonacci(m+1) * where m = r - (n -
 *            r*(r+1)/2), * r is the largest integer with r*(r+1)/2 <= n
 * Range:     u8 n <= 27, u16 n <= 90, u32 n <= 324, u64 n <= 1175,
 *            u128 n <= 4464, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Find the row number r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
static idx_t
row_of(idx_t n)
{
    idx_t lo = 0, hi = n + 1;           /* hi must exceed the answer */
    while (lo + 1 < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        unsigned __int128 t = (unsigned __int128)mid * (mid + 1) / 2;
        if (t <= n)
            lo = mid;
        else
            hi = mid;
    }
    return lo;
}

/* Fast‑doubling algorithm for Fibonacci numbers.
 * It computes (F(n), F(n+1)) and stores them in *fn and *fn1.
 * Works for all seq_t types that support +, *, and subtraction. */
static void
fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn = 0;
        *fn1 = 1;
        return;
    }
    seq_t a, b;
    fib_pair(n >> 1, &a, &b);              /* a = F(k), b = F(k+1) where k = floor(n/2) */

    seq_t t = b + b;                       /* 2*b   */
    seq_t c = a * (t - a);                 /* a*(2*b-a) = F(2k)   */
    seq_t d = a * a + b * b;               /* a^2 + b^2 = F(2k+1) */

    if (n & 1) {                           /* n is odd: (F(2k+1), F(2k+2)) = (d, c + d) */
        *fn = d;
        *fn1 = c + d;
    } else {                               /* n is even: (F(2k), F(2k+1)) = (c, d) */
        *fn = c;
        *fn1 = d;
    }
}

seq_t a(idx_t n)
{
    idx_t r = row_of(n);
    unsigned __int128 tri = (unsigned __int128)r * (r + 1) / 2;
    idx_t k = n - (idx_t)tri;              /* column in the row */
    idx_t m = r - k;                       /* index for the golden rectangle */

    seq_t f_m, f_m_next;
    fib_pair(m, &f_m, &f_m_next);          /* f_m = F(m), f_m_next = F(m+1) */

    return f_m * f_m_next;                 /* value of the triangle entry */
}
