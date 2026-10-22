/*
 * A001682  Numbers k such that 3^k, 3^(k+1) and 3^(k+2) have the same number
 *          of digits.
 * https://oeis.org/A001682
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, nice
 * Formula:   a(n) = n-th k such that 3^k, 3^(k+1), 3^(k+2) have the same
 *            number of digits. This occurs when the fractional part of
 *            k*log10(3) is less than 1 - 2*log10(3)
 * Range:     u8 n <= 12, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n) {
    long double log3 = log10l(3.0L);
    long double threshold = 1.0L - 2.0L * log3;
    
    idx_t count = 0;
    
    for (idx_t k = 0; ; k++) {
        long double val = (long double)k * log3;
        long double frac = val - floorl(val);
        if (frac < threshold) {
            count++;
            if (count == n) {
                return (seq_t)k;
            }
        }
    }
}
