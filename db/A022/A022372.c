/*
 * A022372  Fibonacci sequence beginning 2, 20.
 * https://oeis.org/A022372
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * (Fibonacci(n+2) + 8 * Fibonacci(n))
 * Range:     u8 n <= 6, u16 n <= 18, u32 n <= 41, u64 n <= 87, u128 n <= 179,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Fibonacci uses the standard definition: F(0)=0, F(1)=1 */

#include <stdint.h>   /* for uintptr_t if needed – harmless */

/* Fast doubling Fibonacci: computes F(n) and F(n+1) */
/* Uses only +, -, *, <<, >> operators on seq_t and the constants 0,1,2 */
static void fib_doubling(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0)
    {
        *fn  = 0;
        *fn1 = 1;
        return;
    }
    seq_t a, b;
    fib_doubling(n >> 1, &a, &b);            /* a = F(k), b = F(k+1) */

    /* c = F(2k)   = a * (2*b - a)      */
    /* d = F(2k+1) = a*a + b*b           */
    seq_t two_b_minus_a = ((seq_t)2 * b) - a;
    seq_t c = a * two_b_minus_a;
    seq_t d = a * a + b * b;

    if (n & 1)
    {
        *fn  = d;               /* F(2k+1)   */
        *fn1 = c + d;           /* F(2k+2) = F(2k) + F(2k+1) */
    }
    else
    {
        *fn  = c;               /* F(2k)     */
        *fn1 = d;               /* F(2k+1)   */
    }
}

seq_t a(idx_t n)
{
    seq_t fn, fn1;                   /* F(n), F(n+1) */
    fib_doubling(n, &fn, &fn1);

    seq_t fn2 = fn + fn1;            /* F(n+2) = F(n) + F(n+1) */
    seq_t res = fn2 + fn * (seq_t)8; /* fn2 + 8*F(n) */
    return res << 1;                 /* multiply by 2 */
}
