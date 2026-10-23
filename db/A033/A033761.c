/*
 * A033761  Product t2(q^d); d | 2, where t2 = theta2(q)/(2*q^(1/4)).
 * https://oeis.org/A033761
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of positive odd integer solutions (x,y) to x^2 +
 *            2*y^2 = 8*n + 3
 * Range:     u8 n <= 7, u16 n <= 2855, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root (floor). Works for all unsigned integer types
   and for arbitrary-precision integer classes that support the
   operators used below. */
static inline seq_t isqrt_seq(seq_t x)
{
    seq_t low = 0;
    seq_t high = x;
    while (low <= high) {
        seq_t mid = low + (high - low) / ((seq_t)2);
        seq_t sq  = mid * mid;
        if (sq == x) return mid;
        if (sq < x) {
            low = mid + 1;
        } else {
            /* Here sq > x, so mid is non-zero (unless x==0,
               which is handled by the early return). */
            high = mid - 1;
        }
    }
    return high;          /* floor(sqrt(x)) */
}

seq_t a(idx_t n)
{
    /* m = 8*n + 3 */
    seq_t m = (seq_t)8 * n + 3;

    /* y runs from 1 to floor(sqrt(m/2)) in steps of 2 (odd y). */
    seq_t half = m / ((seq_t)2);
    seq_t y_max = isqrt_seq(half);

    seq_t count = 0;
    for (seq_t y = 1; y <= y_max; y += 2) {
        seq_t t = m - ((seq_t)2) * y * y;
        if (t == 0) continue;        /* skip zero (not a positive solution) */
        seq_t r = isqrt_seq(t);
        if (r * r == t) {
            ++count;                  /* (x = r, y) is a valid solution */
        }
    }
    return count;
}
