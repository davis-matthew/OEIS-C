/*
 * A181895  Difference between maximum and minimum positive value y in
 *          solutions to x^2 - y^2 = n!.
 * https://oeis.org/A181895
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = difference between maximum and minimum positive y in *
 *            solutions to x^2 - y^2 = n! (OEIS A181895)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Validated: partial, pass, 17 terms through n = 20 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow_i32.h"

/* Helper: compute factorials iteratively.
   For built‑in integer types the result may overflow for large n,
   but the tests use only n for which the value fits in seq_t. */
static seq_t factorial(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 2; i <= n; ++i)
        result *= (seq_t)i;
    return result;
}

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

/* Count trailing zeros of a seq_t (i.e., exponent of 2). */
static inline uint64_t trailing_zeros_seq(seq_t x)
{
    uint64_t cnt = 0;
    while ((x & (seq_t)1) == 0) {
        x >>= 1;
        cnt++;
    }
    return cnt;
}

/* Compute v2(n!) = sum_{i>=1} floor(n / 2^i). */
static inline uint64_t v2_factorial(idx_t n)
{
    uint64_t k = 0;
    uint64_t p = 2;
    while (p <= n) {
        k += n / p;
        if (p > n / 2) break;  /* avoid overflow */
        p <<= 1;
    }
    return k;
}

seq_t a(idx_t n)
{
    /* Result for n < 4 is defined as 0 by convention. */
    if (n < 4) {
        return (seq_t)0;
    }

    /* 1. Compute n! */
    seq_t fact = factorial(n);

    /* 2. Compute v2(n!) */
    uint64_t k = v2_factorial(n);   /* k >= 3 for n >= 4 */

    /* 3. Upper bound for divisor: floor_sqrt(n!) */
    seq_t d = floor_sqrt(fact);

    /* 4. Search downward for largest divisor d <= sqrt(fact)
     *    such that d is even and both d and fact/d are even.
     */
    while (1) {
        if ((d & (seq_t)1) == 0) {                 /* d even */
            uint64_t v2_d = trailing_zeros_seq(d);
            if (v2_d <= k - 1) {                  /* fact/d also even */
                if (fact % d == 0) {
                    break;                        /* found suitable divisor */
                }
            }
        }
        if (d == (seq_t)0) {
            /* should not happen for n >= 4 */
            break;
        }
        d -= 1;
    }

    /* 5. Compute minimal and maximal y */
    seq_t min_y = ((fact / d) - d) >> 1;          /* (fact/d - d)/2 */
    seq_t max_y = (fact >> 2) - 1;                /* fact/4 - 1 */

    /* 6. Return the difference */
    return max_y - min_y;
}
