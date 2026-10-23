/*
 * A037214  Expansion of ( Sum_{k>=0} k*q^(k^2) )^2.
 * https://oeis.org/A037214
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of x*y for integers x > 0, y ≥ 0 with x² + y² = n
 * Range:     u8 n <= 184, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root for uint64_t values, using binary search.
   Returns floor( sqrt(value) ). */
static inline idx_t isqrt_u64(idx_t value)
{
    idx_t lo = 0, hi = value;
    while (lo <= hi) {
        idx_t mid = (lo + hi) >> 1;
        idx_t sq  = mid * mid;
        if (sq == value) return mid;
        if (sq < value)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;           /* hi < lo, hi is floor(sqrt(value)) */
}

seq_t a(idx_t n)
{
    /* For n = 0 or 1 the sum is always 0. */
    if (n <= 1) return 0;

    unsigned long long sum = 0;   /* intermediate accumulator */
    idx_t limit = isqrt_u64(n);   /* maximum possible x */

    for (idx_t x = 1; x <= limit; ++x) {
        idx_t x2   = x * x;          /* x², guaranteed to fit in uint64_t */
        idx_t diff = n - x2;         /* n – x² */
        idx_t y    = isqrt_u64(diff);

        if (y * y == diff)          /* check perfect square */
            sum += (unsigned long long)x * (unsigned long long)y;
    }

    return (seq_t)sum;   /* cast back to seq_t; overflow is acceptable
                            when the value cannot be represented */
}
