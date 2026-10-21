/*
 * A098174  a(n) is the smallest e > 0 such that the initial digit of n^e = 1
 *          in decimal representation.
 * https://oeis.org/A098174
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest e > 0 such that the fractional part of
 *            e*log10(n) is less than log10(2). This is equivalent to the
 *            initial decimal digit of n^e being 1
 * Range:     u8 n <= 315, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;           /* undefined for n=0, return 0 */
    const long double log10_2 = log10l(2.0L);
    long double logn = log10l((long double)n);
    long double frac = 0.0L;        /* fractional part of e*log10(n) */

    for (idx_t e = 1; ; ++e) {
        /* add log10(n) and reduce modulo 1 to keep only fractional part */
        frac += logn;
        if (frac >= 1.0L)          /* subtract integral part */
            frac -= floorl(frac);
        if (frac < log10_2)        /* leading digit is 1 */
            return (seq_t)e;
    }
}
