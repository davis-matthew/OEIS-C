/*
 * A164018  The index values of the smallest and the largest n-digit Fibonacci
 *          numbers.
 * https://oeis.org/A164018
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = floor(floor(n/2)*log(10)/log(phi)+log(5)/(2*log(phi))) +
 *            (n mod 2) (n>1) with phi = (1+sqrt(5))/2. Special case: a(1) = 0
 *            (smallest 1‑digit Fibonacci number F(0) = 0)
 * Range:     u8 n <= 106, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;          /* smallest 1‑digit Fibonacci is F(0) = 0 */

    /* Pre‑computed constants using long double */
    static const long double sqrt5  = sqrtl(5.0L);
    static const long double phi    = (1.0L + sqrt5) / 2.0L;
    static const long double log10_phi = logl(10.0L) / logl(phi);          /* ln(10)/ln(phi) */
    static const long double offset    = logl(5.0L) / (2.0L * logl(phi));  /* ln(5)/(2*ln(phi)) */

    idx_t half = n / 2;
    long double val = (long double)half * log10_phi + offset;
    idx_t base = (idx_t)floorl(val + 1e-12L);     /* small epsilon to guard against FP errors */
    base += (n & 1); /* add (n mod 2) */

    return (seq_t)base;
}
