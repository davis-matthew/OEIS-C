/*
 * A135736  Nearest integer to n*Sum_{k=1..n} 1/k = rounded expected coupon
 *          collection numbers.
 * https://oeis.org/A135736
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = round(n * Sum_{k=1..n} 1/k)
 * Range:     u8 n <= 55, u16 n <= 6953, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes the sequence by a straightforward
   floating‑point summation and rounding.  The result is carried
   back into the integer type seq_t via a cast from a native
   signed integer.  It works for all n ≥ 0 and is efficient
   for n up to ~10⁴. */
#include <math.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Accumulate the harmonic sum using double precision. */
    double h = 0.0;
    for (idx_t k = 1; k <= n; ++k) {
        h += 1.0 / (double)k;
    }

    /* Expected number: n * H_n.  Round to nearest integer. */
    double expected = (double)n * h;
    /* floor(value + 0.5) implements round to nearest integer. */
    long long rounded = (long long)(expected + 0.5);

    return (seq_t)rounded;
}
