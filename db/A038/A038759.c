/*
 * A038759  a(n) = ceiling(sqrt(n))*floor(sqrt(n)).
 * https://oeis.org/A038759
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(sqrt(n))*floor(sqrt(n))
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Compute floor(sqrt(n)) for 0 <= n < 2^64 using an integer
   Newton–Raphson method (no floating point, no external helpers). */
static inline idx_t floor_isqrt(idx_t n)
{
    if (n < 2u)            /* sqrt(0)=0, sqrt(1)=1 */
        return n;

    idx_t x = n;
    idx_t y = (x + 1u) >> 1u;
    /* Iterate until convergence: y < x means we can improve */
    while (y < x) {
        x = y;
        /* Avoid division by zero: x is at least 1 here */
        y = (x + n / x) >> 1u;
    }
    return x;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    idx_t k = floor_isqrt(n);      /* floor(sqrt(n))   */
    idx_t kk = k * k;              /* square of floor  */

    if (kk == n)                   /* n is a perfect square  */
        return (seq_t)n;           /* ceil = floor = sqrt(n) */

    /* n is not a square: ceil(sqrt(n)) = k+1  */
    /* Multiply in idx_t to avoid overflow of seq_t (works for all
       seq_t sizes, the product fits into idx_t for UINT64_MAX).  */
    return (seq_t)k * ((seq_t)k + 1u);
}
