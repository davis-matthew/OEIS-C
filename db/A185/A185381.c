/*
 * A185381  a(n) = Fibonacci(k) where k = floor( n*(1+sqrt(5))/2 ).
 * https://oeis.org/A185381
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Fibonacci( floor( (1+sqrt(5))/2 * n ) )
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 29, u64 n <= 58, u128 n <= 115,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square‑root for 128‑bit intermediate values.
   Returns floor( sqrt(x) ). */
static idx_t isqrt128(__int128 x)
{
    __int128 low = 0, high = x;
    while (low <= high)
    {
        __int128 mid = (low + high) >> 1;
        if (mid == 0)
        {
            low = 1;
            continue;
        }
        if (mid <= x / mid)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return (idx_t)high;
}

seq_t a(idx_t n)
{
    /* Compute k = floor( n * φ ) where φ = (1+√5)/2.
       This is equivalent to floor( (n + floor(n*√5)) / 2 ). */
    idx_t sqrt5n = isqrt128((__int128)5 * n * n);   /* floor(n*√5) */
    idx_t k = (n + sqrt5n) >> 1;                    /* floor((n + sqrt5n)/2) */

    /* Compute Fibonacci(k) iteratively: F(0)=0, F(1)=1. */
    if (k == 0)
        return (seq_t)0;

    seq_t f_prev = 0;   /* F(0) */
    seq_t f_curr = 1;   /* F(1) */
    for (idx_t i = 1; i <= k; ++i)
    {
        seq_t f_next = f_prev + f_curr;  /* F(i+1) = F(i) + F(i-1) */
        f_prev = f_curr;                 /* shift for next iteration */
        f_curr = f_next;
    }
    return f_prev;  /* f_prev is F(k) after loop */
}
