/*
 * A061723  Floor of arithmetic-geometric mean of n and 2*n - 1.
 * https://oeis.org/A061723
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, changed
 * Formula:   a(n) = floor(agm(n, 2*n-1))
 * Range:     u8 n <= 176, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the arithmetic‑geometric mean of n and 2*n-1
   using an iterative method with long double precision.
   The floor of the mean is returned as a seq_t. */

#include <math.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* a(0) is defined as 0 */
    if (n == 0)
        return 0;

    /* Convert index to a signed long double for the AM–GM iteration. */
    long double low  = (long double)n;                 /* a0 = n          */
    long double high = 2.0L * low - 1.0L;              /* b0 = 2n-1       */

    for (;;) {
        long double mid_high = (low + high) / 2.0L;     /* arithmetic mean   */
        long double mid_low  = sqrtl(low * high);      /* geometric mean    */

        /* If both bounds fall in the same integer interval,
           the integer part of their common limit is the answer. */
        unsigned long long ah = (unsigned long long)floorl(mid_high);
        unsigned long long al = (unsigned long long)floorl(mid_low);
        if (ah == al)
            return (seq_t)al;

        /* Refine the bounds for the next iteration. */
        low  = mid_low;
        high = mid_high;
    }
}
