/*
 * A097682  E.g.f.: (1/(1-x^8))*exp( 8*sum_{i>=0} x^(8*i+1)/(8*i+1) ) for an
 *          order-8 linear recurrence with varying coefficients.
 * https://oeis.org/A097682
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   for n >= 8, a(n) = 8*a(n-1) + (n!/(n-8)!)*a(n-8); Initial values
 *            (0 <= n <= 7): a(n) = 8^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for malloc / free */

/* calculate a(n) for OEIS sequence A097682 */
seq_t a(idx_t n)
{
    /* for n < 8: simply 8^n */
    if (n < 8) {
        seq_t r = 1;
        for (idx_t i = 0; i < n; ++i)
            r *= (seq_t)8;
        return r;
    }

    /* Circular buffer of the last 8 computed terms */
    seq_t buf[8];
    /* initialise a[0] .. a[7] */
    buf[0] = 1;
    for (idx_t i = 1; i < 8; ++i)
        buf[i] = buf[i - 1] * (seq_t)8;

    /* falling factorial for current n = 8:
          ff = 8*7*6*5*4*3*2*1 = 40320          */
    seq_t ff = 1;
    for (idx_t i = 1; i <= 8; ++i)
        ff *= (seq_t)i;

    /* compute a[8] .. a[n] */
    for (idx_t k = 8; k <= n; ++k) {
        seq_t val = buf[(k - 1) % 8] * (seq_t)8 + ff * buf[(k - 8) % 8];
        buf[k % 8] = val;
        if (k < n) {
            /* prepare ff for the next n:
               ff_next = ff * (k+1) / (k-7)   */
            ff = (ff * (seq_t)(k + 1)) / (seq_t)(k - 7);
        }
    }
    /* result is in the buffer position corresponding to n */
    return buf[n % 8];
}
