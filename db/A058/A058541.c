/*
 * A058541  Trajectory of 1 under map that sends x to 3x - sigma(x).
 * https://oeis.org/A058541
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - σ(a(n-1)), a(0) = 1 (σ(m) is the sum of all
 *            positive divisors of m) The sequence quickly falls into the
 *            fixed point 28
 * Range:     u8 n <= 12, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute σ(m) for 0 ≤ m ≤ 2⁶⁴-1 (works for the small values
   that occur in the trajectory).  Only integer arithmetic is used. */
static seq_t
sigma_of_uint64 (uint64_t m)
{
    seq_t sum = 0;
    uint64_t i = 1;
    for (; i * i <= m; ++i)
    {
        if (m % i == 0)
        {
            sum += (seq_t)i;
            uint64_t j = m / i;
            if (j != i)
                sum += (seq_t)j;
        }
    }
    return sum;
}

seq_t a (idx_t n)
{
    /* The numbers converge to 28 after a few iterations.  */
    const seq_t TARGET = 28;

    seq_t x = 1;               /* a(0) */
    idx_t steps = n;

    /* We only need to iterate up to 20 steps: after that x == 28. */
    if (steps > 20)
        steps = 20;

    for (idx_t i = 0; i < steps; ++i)
    {
        if (x == TARGET)
            break;
        seq_t sigma = sigma_of_uint64 ((uint64_t)x);
        x = (seq_t)3 * x - sigma;
    }

    return x;
}
