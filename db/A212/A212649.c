/*
 * A212649  a(n) = floor(Sum_{k=0..n-1} sqrt(n^2 - k^2)).
 * https://oeis.org/A212649
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor( Sum_{k=0}^{n-1} sqrt(n*n - k*k) )
 * Range:     u8 n <= 17, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1.  For safety, return 0 for n == 0. */
    if (n == 0) return (seq_t)0;

    double dn = (double)n;
    double sum = 0.0;
    for (idx_t k = 0; k < n; ++k)
    {
        double t = dn * dn - (double)k * (double)k; /* t ≥ 0 */
        sum += sqrt(t);
    }
    /* Add a tiny epsilon to guard against floating‑point round‑off that
       could make an integer value appear slightly below the true sum.
       The epsilon is far smaller than 1.0, so it does not affect the
       floor unless the true sum is within ~1e-9 of an integer,
       which is exceedingly unlikely for these values. */
    unsigned long long res = (unsigned long long)(sum + 1e-9);
    return (seq_t)res;
}
