/*
 * A101355  a(n) = binomial(2*binomial(2*n,n-1),binomial(2*n,n-1)-1).
 * https://oeis.org/A101355
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(2*binomial(2*n,n-1),binomial(2*n,n-1)-1)
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Validated: partial, pass, 5 terms through n = 5 (2026-11-01)
 * Helpers:   binomial_u64_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial_u64_to_seq.h"

static seq_t binom_big(seq_t m, seq_t k)
{
    if (k > m) return 0;
    /* use symmetry to reduce k */
    if (k > m - k) k = m - k;
    /* if k fits into uint64_t, iterate */
    if (k > (seq_t)UINT64_MAX) return 0; /* too large to compute iteratively */
    seq_t res = 1;
    for (idx_t i = 1; i <= (idx_t)k; ++i)
    {
        res = res * (m - k + (seq_t)i);
        res = res / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0; /* sequence is defined for n >= 1 */
    seq_t m1 = (seq_t)2 * (seq_t)n;
    seq_t k1 = (seq_t)(n - 1);
    seq_t M   = binom_big(m1, k1);
    seq_t m2  = M + M;           /* 2*M */
    seq_t k2  = M - (seq_t)1;    /* M-1 */
    return binom_big(m2, k2);
}
