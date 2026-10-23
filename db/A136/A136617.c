/*
 * A136617  a(n) = largest k such that the sum of k consecutive reciprocals
 *          1/n + ... + 1/(n+k-1) does not exceed 1.
 * https://oeis.org/A136617
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = largest k such that 1/n + 1/(n+1) + … + 1/(n+k-1) ≤ 1
 * Range:     u8 n <= 149, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /*  Use a running double sum to avoid big rational arithmetic.  For
        the values of n considered (>0, smallish) double precision is
        more than sufficient to distinguish consecutive integral sums
        because the step size is at least 1/(n+k) and is well above
        machine epsilon. */
    double sum = 0.0;
    idx_t k    = 0;
    for (idx_t i = n;; ++i) {
        double term = 1.0 / (double)i;
        if (sum + term > 1.0) break;
        sum += term;
        ++k;
    }
    return (seq_t)k;
}
