/*
 * A045521  Numbers k such that k! has initial digit '2'.
 * https://oeis.org/A045521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth positive integer k such that the leading digit
 *            of k! is 2. The test is done by maintaining the cumulative sum
 *            of log10(i) and checking whether its fractional part lies in
 *            [log10(2), log10(3)). The function recomputes from scratch on
 *            each call, which is fast enough for n up to abou...
 * Range:     u8 n <= 47, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return 0;          /* offset starts at 1 */
    const double log10_2 = log10(2.0);
    const double log10_3 = log10(3.0);

    double logsum = 0.0;
    idx_t   k      = 0;
    idx_t   count  = 0;

    while (count < n) {
        ++k;
        logsum += log10((double)k);
        double frac = logsum - floor(logsum);
        if (frac >= log10_2 && frac < log10_3) {
            ++count;
            if (count == n)
                return (seq_t)k;
        }
    }
    return 0;   /* never reached */
}
