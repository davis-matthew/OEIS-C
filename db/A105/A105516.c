/*
 * A105516  Number of times 6 is the leading digit of the first n+1 Fibonacci
 *          numbers in decimal representation.
 * https://oeis.org/A105516
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = #{ k : 0 ≤ k ≤ n and the first decimal digit of
 *            Fibonacci(k) is 6 }
 * Range:     u8 n <= 3842, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The leading digit of Fibonacci(k) is computed from the logarithmic identity
   leading_digit(F(k)) = floor(10^{ frac( k*log10(phi) - log10(sqrt(5)) ) })
   which is valid for k ≥ 2.  For k = 0 and k = 1 we handle the cases
   explicitly.  The computation uses only integer arithmetic for the result
   and floating‑point operations only for intermediate values. */
#include <math.h>

seq_t a(idx_t n)
{
    seq_t count = 0;

    /* constants for long double precision */
    const long double log10_phi    = log10l((1.0L + sqrtl(5.0L)) / 2.0L);
    const long double log10_sqrt5  = log10l(sqrtl(5.0L));

    for (idx_t k = 0; k <= n; ++k) {
        int digit;
        if (k == 0) {
            /* Fibonacci(0) = 0, leading digit is 0 */
            digit = 0;
        } else if (k == 1) {
            /* Fibonacci(1) = 1 */
            digit = 1;
        } else {
            /* logarithmic computation for k ≥ 2 */
            long double val = (long double)k * log10_phi - log10_sqrt5;
            long double frac = val - floorl(val);
            /* small epsilon to avoid rounding down when frac is very close
               to 1.0 due to numeric errors. */
            int d = (int)(powl(10.0L, frac) + 1e-12L);
            digit = d;
        }
        if (digit == 6)
            ++count;
    }

    return (seq_t)count;
}
