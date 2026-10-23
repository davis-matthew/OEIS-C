/*
 * A033482  Expansion of (1+2*x-3*x^2-4*x^3+x^4)/(1-8*x^2+11*x^4).
 * https://oeis.org/A033482
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-2) - 11*a(n-4) (for n >= 5) a(0)=1, a(1)=2, a(2)=5,
 *            a(3)=12, a(4)=30
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const seq_t init_vals[5] = { (seq_t)1, (seq_t)2, (seq_t)5, (seq_t)12, (seq_t)30 };

seq_t a(idx_t n)
{
    if (n < 5)
        return init_vals[n];

    /* small circular buffer of last 5 computed values */
    seq_t ring[5];
    ring[0] = init_vals[0];
    ring[1] = init_vals[1];
    ring[2] = init_vals[2];
    ring[3] = init_vals[3];
    ring[4] = init_vals[4];

    for (idx_t i = 5; i <= n; ++i)
    {
        /* a(i) = 8*a(i-2) - 11*a(i-4) */
        seq_t val = (seq_t)8 * ring[(i - 2) % 5] - (seq_t)11 * ring[(i - 4) % 5];
        ring[i % 5] = val;
    }
    return ring[n % 5];
}
