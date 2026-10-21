/*
 * A061508  Smallest positive m such that n^m has at least n digits.
 * https://oeis.org/A061508
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = ceiling((n-1)/log10(n))
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (with a(1) = 1)                                               */
#include <stdint.h>
#include <math.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;      /* a(1) = 1 */

    /* Use long double to compute logarithm and the estimate.   */
    long double ln10 = log10l((long double)n);

    /* Exact real value of (n-1)/log10(n).                     */
    long double exact = ((long double)(n - 1)) / ln10;

    /* Upper bound: ceiling of the exact value                */
    unsigned long long m = (unsigned long long)ceill(exact);

    /* Reduce m while the digit count of n^(m-1) is still at
       least n. The digit count of n^k equals
       floor(k*log10(n)) + 1, so we can test it without computing
       the full power.                                          */
    while (m > 1) {
        long double d = ((long double)(m - 1)) * ln10;
        /* Use a tiny epsilon to guard against rounding errors. */
        unsigned long long digits = (unsigned long long)floorl(d + 1e-12L) + 1;
        if (digits < n)
            break;          /* m-1 is too small */
        --m;               /* try a smaller exponent */
    }

    return (seq_t)m;
}
