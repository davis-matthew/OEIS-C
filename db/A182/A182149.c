/*
 * A182149  a(n) = Fibonacci(n^3).
 * https://oeis.org/A182149
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(n^3)
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute Fibonacci of a non‑negative integer k using fast doubling.  */
static seq_t fib_idx(idx_t k)
{
    if (k == 0)
        return (seq_t)0;

    seq_t a = 0;   /* F(0)  */
    seq_t b = 1;   /* F(1)  */

    /* Find highest set bit of k (floor(log2(k))) */
    int msb = 0;
    for (idx_t t = k; t > 1; t >>= 1)
        msb++;

    for (int i = msb; i >= 0; --i)
    {
        /* d = F(2*i) = a * (2*b - a) */
        seq_t two_b = b + b;
        seq_t d = a * (two_b - a);

        /* e = F(2*i+1) = a*a + b*b */
        seq_t e = a * a + b * b;

        if ((k >> i) & 1)
        {
            a = e;
            b = d + e;
        }
        else
        {
            a = d;
            b = e;
        }
    }
    return a;          /* F(k) */
}

seq_t a(idx_t n)
{
    /* n^3 fits into idx_t for the ranges considered (n <= ~10^4). */
    idx_t cube = n * n * n;
    return fib_idx(cube);
}
