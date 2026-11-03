/*
 * A133827  Number of solutions to x + 7 * y = 2 * n in triangular numbers.
 * https://oeis.org/A133827
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of pairs of triangular numbers (x,y) satisfying x
 *            + 7*y = 2*n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 103 terms through n = 9685 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* a(n) -------------------------------------------------------------- */
/* Input:   n  (idx_t)  non‑negative index                     */
/* Output:  a(n) (seq_t) number of solutions                        */
seq_t a(idx_t n)
{
    /* two_n = 2 * n (use idx_t to avoid overflow when n is large) */
    idx_t two_n = (idx_t)2 * n;

    seq_t count = 0;          /* result to return */

    /* iterate over triangular numbers y = t_k = k(k+1)/2  */
    for (idx_t k = 0; ; ++k)
    {
        /* t_k = k(k+1)/2 */
        idx_t t = k * (k + 1) / 2;

        /* 7 * y = 7 * t_k */
        idx_t seven_t = (idx_t)7 * t;

        /* once 7*y exceeds 2*n no further solutions possible */
        if (seven_t > two_n)
            break;

        /* remaining part: 2*n - 7*y must itself be triangular */
        idx_t rem = two_n - seven_t;

        /* t is triangular iff 8*t + 1 is a perfect square */
        idx_t val = rem * (idx_t)8 + 1;

        /* integer sqrt of val using double as an intermediate.
           For val <= 2^53 the conversion is exact.  For larger
           val the loop below corrects any rounding error.     */
        idx_t r = (idx_t)sqrt((double)val);
        while ((r + 1) * (r + 1) <= val)
            ++r;
        while (r * r > val)
            --r;

        if (r * r == val)
            count = count + (seq_t)1;   /* one valid solution found */
    }

    return count;
}
