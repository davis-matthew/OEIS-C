/*
 * A184726  a(n) = A005408(n-1)/A090368(n-1) for n > 2 and a(n) = 0 for n <=
 *          2.
 * https://oeis.org/A184726
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*n-3)/spf(2*n-3) for n > 2, otherwise a(n) = 0 where
 *            spf(m) is the smallest prime factor of m (m > 1)
 * Range:     u8 n <= 364, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Smallest prime factor of an odd number > 1.
   For an odd m, spf(m) is the smallest odd divisor of m.          */
static seq_t _spf(seq_t m)
{
    /* m is odd, but handle the even case for robustness.              */
    if ((m & (seq_t)1) == 0) return (seq_t)2;
    seq_t d = (seq_t)3;
    /* iterate while d <= m/d to avoid overflow of d*d           */
    while (d <= m / d) {
        if ((m % d) == (seq_t)0) return d;
        d += (seq_t)2;
    }
    /* m was prime                                    */
    return m;
}

seq_t a(idx_t n)
{
    if (n <= (idx_t)2) return (seq_t)0;
    /* Compute odd number 2*n-3 in seq_t to avoid intermediate overflow.   */
    seq_t x = (seq_t)n * (seq_t)2U - (seq_t)3U;
    seq_t spf = _spf(x);
    return x / spf;
}
