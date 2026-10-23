/*
 * A018863  Smallest power of 6 that begins with n.
 * https://oeis.org/A018863
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest power of 6 that begins with n
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   big_pow, digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/digits.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;          /* a(1) = 1 */

    /* Detect if n is a power of 6 */
    idx_t tmp = n;
    idx_t e = 0;
    while (tmp % 6 == 0 && tmp > 1) {
        tmp /= 6;
        ++e;
    }
    if (tmp == 1) {
        return big_pow((seq_t)6, (seq_t)e);   /* a(6^k) = 6^k */
    }

    /* Number of decimal digits of n */
    int D = digits(n);
    /* Logarithmic window for the leading digits of 6^e */
    double nd = (double)n;
    double a_window = log10(nd) - (double)(D - 1);
    double b_window = log10(nd + 1.0) - (double)(D - 1);

    /* log10(6), constant to avoid repeated calls to log10(6) */
    const double LOG10_6 = 0.778151250383643632; /* log10(6) */
    double frac = 0.0;   /* fractional part of e * log10(6) */

    /* Search for the smallest exponent e>0 satisfying the condition */
    for (idx_t exp = 1; ; ++exp) {
        frac += LOG10_6;
        if (frac >= 1.0) frac -= floor(frac);   /* keep within [0,1) */
        if (frac >= a_window && frac < b_window) {
            return big_pow((seq_t)6, (seq_t)exp);
        }
    }
}
