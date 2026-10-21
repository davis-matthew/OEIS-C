/*
 * A216377  The most significant digit in base n representation of n!.
 * https://oeis.org/A216377
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = most significant digit of n! taken in base n i.e.
 *            modlg(n!, n) = floor(n! / n^{floor(log_n(n!))})
 * Range:     u8 n <= 272, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                     /* undefined for n < 2, returns 0 */

    /* Compute the natural logarithms of factorial and base */
    double ln_n = log((double)n);
    double sum = 0.0;
    for (idx_t i = 2; i <= n; ++i)
        sum += log((double)i);

    /* Compute log_n(n!) = sum / ln(n)  */
    double k_frac = sum / ln_n;              /* fractional log_n(n!) */
    long long k = (long long)floor(k_frac);  /* integer part */
    double frac = k_frac - (double)k;        /* fractional part */

    /* Leading digit = floor(n^frac) */
    double val = pow((double)n, frac);
    long long digit = (long long)(val + 1e-12);  /* tiny epsilon to avoid 0.999... errors */

    if (digit < 1)
        digit = 1;
    if (digit >= (long long)n)
        digit = (long long)n - 1;

    return (seq_t)digit;
}
