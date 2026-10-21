/*
 * A094565  Triangle read by rows: binary products of Fibonacci numbers.
 * https://oeis.org/A094565
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 78, u32 n <= 299, u64 n <= 1127,
 *            u128 n <= 4371, big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Triangle read by rows: binary products of Fibonacci numbers.
    a(n) = F(2*k) * F(2*r - 2*k + 1) where
          (r-1)*r/2 < n ≤ r*(r+1)/2   (row number r)
          k = n - (r-1)*r/2            (position in the row, 1‑based)   */

#include "../utils/fib.h"   /* provides seq_t fib(idx_t n) */

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;   /* safety for index 0, not used */

    /* find smallest r with r*(r+1)/2 ≥ n (binary search) */
    idx_t low = 1, high = 1;
    while ((high * (high + 1)) / 2 < n) high <<= 1;   /* double until enough */

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ((mid * (mid + 1)) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;                         /* row number */

    /* position k in that row (1‑based) */
    idx_t k = n - (r * (r - 1)) / 2;

    /* Fibonacci values needed */
    seq_t f_even = fib(2 * k);                 /* F(2k)   */
    seq_t f_odd  = fib(2 * r - 2 * k + 1);     /* F(2r-2k+1) */

    return f_even * f_odd;                     /* product */
}
