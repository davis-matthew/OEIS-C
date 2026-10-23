/*
 * A100665  a(n) = round(sqrt(Fibonacci(n))).
 * https://oeis.org/A100665
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round(sqrt(Fibonacci(n)))
 * Range:     u8 n <= 13, u16 n <= 24, u32 n <= 47, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   bitlen, isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses fast‑doubling Fibonacci and an integer square–root routine */
#include "../utils/isqrt.h"
#include "../utils/bitlen.h"

/* Compute F(n) exactly.  Returns F(n) for n >= 0. */
static seq_t fib_fast(idx_t n)
{
    if (n == 0) return (seq_t)0;
    seq_t a = (seq_t)0;            /* F(k)   */
    seq_t b = (seq_t)1;            /* F(k+1) */
    idx_t bl = bitlen(n);          /* number of bits of n */

    /* Process bits from most‑significant to least‑significant */
    for (idx_t i = bl; i > 0; --i)
    {
        /* c = F(2k) = a * (2*b - a) */
        seq_t c = a * (((b << 1) - a));
        /* d = F(2k+1) = a^2 + b^2 */
        seq_t d = a * a + b * b;

        /* bit of n at position i-1 (0‑based) */
        if ((n >> (i - 1)) & 1ULL)
        {
            a = d;
            b = c + d;
        }
        else
        {
            a = c;
            b = d;
        }
    }
    return a;      /* F(n) */
}

/* Sequence definition: a(n) = round(sqrt(Fibonacci(n))) */
seq_t a(idx_t n)
{
    seq_t f = fib_fast(n);          /* exact Fibonacci(n) */
    seq_t r = isqrt(f);             /* floor(sqrt(F(n))) */

    /* Decide whether to round up: 4*f >= 4*r^2 + 4*r + 1 */
    seq_t four_f = f << 2;          /* 4 * f       */
    seq_t r2     = r * r;           /* r^2         */
    seq_t right  = (r2 << 2) + (r << 2) + 1ull; /* 4*r^2 + 4*r + 1 */

    if (four_f >= right)
        return r + 1;
    return r;
}
