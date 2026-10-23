/*
 * A108141  Least k such that the number (n+1)(n+2)(n+3)...(n+k) >= n^n.
 * https://oeis.org/A108141
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min{k≥1 : (n+1)(n+2)...(n+k) ≥ nⁿ}
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    /* Target logarithm: log₂(nⁿ) = n·log₂(n) */
    long double target = (long double)n * log2l((long double)n);

    /* Accumulate logarithms of the incremental terms (n+1), (n+2), … */
    long double acc = 0.0L;
    idx_t      k  = 0;

    while (1) {
        ++k;
        acc += log2l((long double)(n + k));
        if (acc >= target)
            return (seq_t)k;
    }
}
