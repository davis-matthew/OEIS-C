/*
 * A066508  a(n) = floor(Sum_{i=1..n} (1/i)^(1/i)).
 * https://oeis.org/A066508
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor( Σ_{i=1}^{n} i^{-1/i} )
 * Range:     u8 n <= 270, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    long double sum = 0.0L;
    for (idx_t i = 1; i <= n; ++i)
    {
        long double term = expl(-logl((long double)i) / (long double)i);
        sum += term;
    }
    /* Add a tiny epsilon to guard against floating‑point rounding that
       could bring the value just below an integer. */
    long long floor_sum = (long long)floorl(sum + 1e-12L);
    return (seq_t)floor_sum;
}
