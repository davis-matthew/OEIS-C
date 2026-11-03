/*
 * A101356  Binomial( 2*binomial(2*n,n-1),n-1).
 * https://oeis.org/A101356
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Binomial( 2*binomial( 2*n+2 , n ) , n )
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 8,
 *            big any n
 * Validated: partial, pass, 10 terms through n = 9 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <sys/types.h>
#include "../utils/binomial.h"

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

seq_t a(idx_t n) {
    /* inner binomial C(2*n+2, n) */
    idx_t N = 2*n + 2;
    seq_t inner = binomial(N, n);
    /* top of outer binomial: 2 * inner */
    seq_t top = inner * 2;
    /* outer binomial C(top, n) */
    seq_t result = binom_big(top, (seq_t)n);
    return result;
}
