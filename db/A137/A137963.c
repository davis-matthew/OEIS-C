/*
 * A137963  G.f. satisfies A(x) = 1 + x*(1 + x*A(x)^3)^5.
 * https://oeis.org/A137963
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 22, u128 n <= 42,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A137963
   Formula: a(n) = Sum_{k=0..n-1} C(5*(n-k), k)/(n-k) * C(3*k, n-k-1),
   with a(0) = 1
*/

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t sum = (seq_t)0;
    for (idx_t k = 0; k < n; ++k) {
        idx_t m = n - k;                       /* m = n-k  */
        seq_t bin1 = binom(5 * m, k);          /* C(5m, k) */
        seq_t bin2 = binom(3 * k, m - 1);      /* C(3k, m-1) */

        /* Compute term = (bin1 * bin2) / m.
           The division is exact by definition of the sequence. */
        seq_t term = (bin1 * bin2) / (seq_t)m;

        sum += term;
    }
    return sum;
}
