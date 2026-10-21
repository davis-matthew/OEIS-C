/*
 * A094571  Triangle of binary sums of squares of Fibonacci numbers.
 * https://oeis.org/A094571
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = F(k)^2 + F(r-k)^2 where r = floor((sqrt(8n+1)-1)/2) and k
 *            = n - r(r+1)/2, F(m) = m-th Fibonacci number (F(0)=0, F(1)=1)
 * Range:     u8 n <= 35, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Return the row index r such that r*(r+1)/2 <= n < (r+1)(r+2)/2. */
static idx_t row_of(idx_t n)
{
    idx_t low = 0, high = n;
    while (low <= high)
    {
        idx_t mid = low + (high - low) / 2;
        __int128 t = (__int128)mid * (mid + 1) / 2;
        if (t <= n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

/* Fast doubling Fibonacci: returns F(n) in *fn and F(n+1) in *fn1. */
static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0)
    {
        *fn = 0;
        *fn1 = 1;
        return;
    }
    seq_t a, b;
    fib_pair(n >> 1, &a, &b);                /* a = F(k), b = F(k+1) where k = n/2 */
    seq_t two_b = b + b;                     /* 2*F(k+1) */
    seq_t c = a * (two_b - a);               /* F(2k)     */
    seq_t d = a * a + b * b;                 /* F(2k+1)   */
    if (n & 1)
    {
        *fn  = d;                            /* F(2k+1)   */
        *fn1 = c + d;                        /* F(2k+2)   */
    }
    else
    {
        *fn  = c;                            /* F(2k)     */
        *fn1 = d;                            /* F(2k+1)   */
    }
}

/* External linkage function requested by the problem statement. */
seq_t a(idx_t n)
{
    idx_t r = row_of(n);                     /* row index   */
    idx_t base = r * (r + 1) / 2;            /* cumulative entries before row r */
    idx_t k = n - base;                      /* column index within row r (0‑based) */

    seq_t fk, fk1, fk2, fk1b;                /* scratch for Fibonacci values */

    fib_pair(k,    &fk,  &fk1);              /* F(k), F(k+1)   */
    fib_pair(r - k, &fk2, &fk1b);            /* F(r-k), F(r-k+1) */

    seq_t fk_sq = fk * fk;
    seq_t fk2_sq = fk2 * fk2;
    return fk_sq + fk2_sq;
}
