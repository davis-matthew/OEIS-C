/*
 * A089668  a(n) = S2(n,5), where S2(n, t) = Sum_{k=0..n} k^t *(Sum_{j=0..k}
 *          binomial(n,j))^2.
 * https://oeis.org/A089668
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} k^5 * (Sum_{j=0..k} binomial(n,j))^2
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 20, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t if needed by the build environment */

seq_t a(idx_t n)
{
    seq_t sum = 0;      /* accumulating result */
    seq_t bin = 1;      /* binomial(n,0) */
    seq_t cum = 0;      /* cumulative sum of binomials up to current k */

    for (idx_t k = 0; k <= n; ++k) {
        /* add current binomial to cumulative sum */
        cum += bin;

        /* compute k^5 (k is small, we can compute directly) */
        seq_t kseq = (seq_t)k;
        seq_t k2   = kseq * kseq;          /* k^2   */
        seq_t k3   = k2 * kseq;            /* k^3   */
        seq_t k4   = k3 * kseq;            /* k^4   */
        seq_t k5   = k4 * kseq;            /* k^5   */

        /* square cumulative sum */
        seq_t cum2 = cum * cum;

        /* term for this k */
        seq_t term = k5 * cum2;
        sum += term;

        /* update binomial to next value binomial(n,k+1) */
        if (k < n) {
            seq_t mult = (seq_t)(n - k);   /* numerator factor (n-k) */
            seq_t next = bin * mult;       /* multiply then divide */
            next /= (seq_t)(k + 1);        /* exact division */
            bin = next;
        }
    }

    return sum;
}
