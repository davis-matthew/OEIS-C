/*
 * A108350  Number triangle T(n,k) = Sum_{j=0..n-k}
 *          binomial(k,j)*binomial(n-j,k)*((j+1) mod 2).
 * https://oeis.org/A108350
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = T(m,k) where m = floor((sqrt(8n+1)-1)/2) (the row of the
 *            triangle) k = n - m*(m+1)/2 (the column in that row) T(m,k) =
 *            Σ_{j=0..m-k, j even} binom(k,j) * binom(m-j,k)
 * Range:     u8 n <= 48, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Validated: partial, pass, 76 terms through n = 75 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is obtained by reading the triangle row‑wise.  */
#include <sys/types.h>
#include "../utils/binomial.h"

static inline seq_t T_value(idx_t m, idx_t k)
{
    seq_t sum = 0;
    idx_t diff = m - k;
    for (idx_t j = 0; j <= diff; j += 2) {
        seq_t b1 = binomial(k, j);
        seq_t b2 = binomial(m - j, k);
        sum += b1 * b2;
    }
    return sum;
}

seq_t a(idx_t n)
{
    /* Find the row m such that m*(m+1)/2 <= n < (m+1)*(m+2)/2. */
    idx_t m = 0;
    while ((m + 1) * (m + 2) / 2 <= n) ++m;
    /* k = column index within row m */
    idx_t k = n - m * (m + 1) / 2;

    return T_value(m, k);
}
