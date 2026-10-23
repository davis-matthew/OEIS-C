/*
 * A025744  Index of 10^n within the sequence of the numbers of the form
 *          6^i*10^j.
 * https://oeis.org/A025744
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + \sum_{k=0}^{n} \lceil k \log_6(10) \rceil \n where
 *            \log_6(10) = 1 / \log_{10}(6)
 * Range:     u8 n <= 19, u16 n <= 318, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>            /* ceill, log10l */

seq_t a(idx_t n)
{
    /*   c = log_6(10) = 1 / log_10(6)   */
    static const long double c = 1.0L / log10l(6.0L);

    unsigned long long sum = 1ULL;           /* a(0) = 1           */
    for (idx_t k = 1; k <= n; ++k) {
        unsigned long long term = (unsigned long long)ceill(k * c);
        sum += term;
    }
    return (seq_t)sum;                       /* cast works for all seq_t */
}
