/*
 * A223528  Triangle S(n,k) by rows: coefficients of 4^(n/2)*(x^(3/4)*d/dx)^n
 *          when n=0,2,4,6,...
 * https://oeis.org/A223528
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   The triangle S(m,k) (m = n/2, 0 <= k <= m) is S(m,k) = 4^k *
 *            C(m,k) * ∏_{j=k+1}^{m} (4j-3). The sequence a(n) enumerates the
 *            triangle row‑wise, with offset 1. That is, for a given n >= 1
 *            find the unique m such that (m+1)(m+2)/2 >= n. Then k = n -
 *            m(m+1)/2 - 1 and a(n) = S(m,k)
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   binomial, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"    /* provides binomial(idx_t a, idx_t b) */
#include "../utils/seq_pow.h"     /* provides seq_pow(seq_t base, idx_t exp) */

seq_t a(idx_t n)
{
    /* Locate the row m (0‑based) that contains the term n.  */
    idx_t m = 0;
    while ((m + 1) * (m + 2) / 2 < n)
        ++m;

    /* Index k within the row (0‑based). */
    idx_t k = n - m * (m + 1) / 2 - 1;

    /* 4^k */
    seq_t pow4k = seq_pow((seq_t)4, k);

    /* Binomial coefficient C(m,k) */
    seq_t bin = binomial(m, k);

    /* Product ∏_{j=k+1}^{m} (4j-3)  */
    seq_t prod = 1;
    for (idx_t j = k + 1; j <= m; ++j)
        prod *= (seq_t)(4 * j - 3);

    /* Final term: 4^k * C(m,k) * product */
    return pow4k * bin * prod;
}
