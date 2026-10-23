/*
 * A022342  Integers with "even" Zeckendorf expansions (do not end with ... +
 *          F_2 = ... + 1) (the Fibonacci-even numbers); also, apart from
 *          first term, a(n) = Fibonacci successor to n-1.
 * https://oeis.org/A022342
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = floor(n*phi) - 1 where phi = (1+sqrt(5))/2
 * Range:     u8 n <= 3, u16 n <= 17, u32 n <= 6606, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root: returns ⌊sqrt(x)⌋ for any unsigned integer type seq_t.
   Works for built-in unsigned types up to 128 bits and for arbitrary‑precision
   integer classes with the usual arithmetic operators. */
static seq_t isqrt(seq_t x)
{
    seq_t low = 0;
    seq_t high = x;
    while (low <= high)
    {
        seq_t mid = (low + high) >> 1;
        seq_t sq  = mid * mid;
        if (sq <= x)
        {
            low = mid + 1;
        }
        else
        {
            if (mid == 0) break;  /* avoid underflow */
            high = mid - 1;
        }
    }
    return high;
}

seq_t a(idx_t n)
{
    /* n is guaranteed >= 1 by the caller */
    seq_t N = (seq_t)n;                     /* cast to seq_t first */
    seq_t five_n_sq = (seq_t)5 * N * N;     /* 5*n*n */
    seq_t s = isqrt(five_n_sq);             /* floor(sqrt(5*n*n)) = floor(n*sqrt5) */
    seq_t sum = N + s;                      /* n + floor(n*sqrt5) */
    seq_t div2 = sum / 2;                   /* floor((n + floor(n*sqrt5))/2) */
    return div2 - (seq_t)1;                 /* a(n) = floor(n*phi) - 1 */
}
