/*
 * A225545  Floor(exp(w(n))) where W is the Lambert W function.
 * https://oeis.org/A225545
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(exp(W(n))) = floor(n / W(n)) Equivalent integer
 *            characterization: a(n) is the largest integer k such that k *
 *            ln(k) <= n
 * Range:     u8 n <= 1419, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    /* Special cases for small n */
    if (n <= 1)
        return (seq_t)1;

    /* Binary search for the largest k with k*ln(k) <= n */
    idx_t lo = 1, hi = n;          /* hi is surely large enough */
    while (lo < hi) {
        idx_t mid = lo + (hi - lo + 1) / 2;    /* mid >= lo */
        double val = (double)mid * log((double)mid);
        if (val <= (double)n)
            lo = mid;          /* mid is possible answer */
        else
            hi = mid - 1;      /* mid too large */
    }
    return (seq_t)lo;
}
