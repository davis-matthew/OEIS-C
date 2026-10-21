/*
 * A099236  Sums of antidiagonals of A099233.
 * https://oeis.org/A099236
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sum_{j=0..n} sum_{k=0..n-j} binomial(j*(n-j-k), k)
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 21, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    seq_t res = 0;
    for (idx_t j = 0; j <= n; ++j) {
        idx_t t = n - j;                 /* t = n - j */
        /* Compute the largest k such that k <= j*(t-k)
           -> k*(j+1) <= j*t  -> k_max = floor(j*t/(j+1))  */
        idx_t numer = j * t;
        idx_t denom = j + 1;
        idx_t kmax = numer / denom;      /* inclusive upper limit on k */
        idx_t m = numer;                 /* m = j*t   (i.e., j*(t-0)) */
        for (idx_t k = 0; k <= kmax; ++k) {
            res += binom(m, k);          /* binomial(j*(t-k), k) */
            m -= j;                      /* decrement m for next k */
        }
    }
    return res;
}
