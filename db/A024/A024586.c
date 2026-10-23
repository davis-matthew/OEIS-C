/*
 * A024586  a(n) = floor(Sum_{k=1..n} of 1/{k*Pi}) where { } denotes
 *          fractional part.
 * https://oeis.org/A024586
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(∑_{k=1..n} 1 / frac(k·π))
 * Range:     u8 n <= 72, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* frac(x) denotes the fractional part of x. */

#include <math.h>

seq_t a(idx_t n)
{
    double sum = 0.0;
    const double PI = 3.141592653589793238462643383279502884L;

    for (idx_t k = 1; k <= n; ++k) {
        double x = k * PI;
        double f = x - floor(x);         /* fractional part of k·π */
        sum += 1.0 / f;                  /* add reciprocal */
    }

    unsigned long long val = (unsigned long long) floor(sum);
    return (seq_t) val;
}
