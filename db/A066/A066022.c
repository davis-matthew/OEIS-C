/*
 * A066022  Number of digits in n^n.
 * https://oeis.org/A066022
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = floor(n*log10(n)) + 1 (number of decimal digits of nⁿ)
 * Range:     u8 n <= 122, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    /* OEIS offset starts at 1; n is guaranteed >= 1 */
    if (n == 0) return 0;                 /* safety for unexpected calls */

    /* Compute n*log10(n) with long double for sufficient precision.
       Adding a tiny epsilon ensures that values very close to an integer
       are rounded up before truncation, giving the correct floor. */
    long double x = (long double)n * log10l((long double)n);
    unsigned long long d = (unsigned long long)(x + 1e-12L); /* floor(x) */

    return (seq_t)(d + 1);                /* number of digits */
}
